/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "TLSSocketWrapper.h"
#include "drivers/Timer.h"

#define TRACE_GROUP "TLSW"
#include "mbed-trace/mbed_trace.h"
#include "mbedtls/debug.h"

TLSSocketWrapper::TLSSocketWrapper(Socket *transport, const char *hostname, control_transport control) :
    _transport(transport),
    _cacert(NULL),
    _clicert(NULL),
    _ssl_conf(NULL),
    _connect_transport(control==TRANSPORT_CONNECT || control==TRANSPORT_CONNECT_AND_CLOSE),
    _close_transport(control==TRANSPORT_CLOSE || control==TRANSPORT_CONNECT_AND_CLOSE),
    _handshake_completed(false),
    _cacert_allocated(false),
    _clicert_allocated(false),
    _ssl_conf_allocated(false)
{
    mbedtls_entropy_init(&_entropy);
    mbedtls_ctr_drbg_init(&_ctr_drbg);
    mbedtls_ssl_init(&_ssl);
    mbedtls_pk_init(&_pkctx);

    if (hostname) {
        set_hostname(hostname);
    }
}

TLSSocketWrapper::~TLSSocketWrapper() {
    if (_transport) {
        close();
    }
    mbedtls_entropy_free(&_entropy);
    mbedtls_ctr_drbg_free(&_ctr_drbg);
    mbedtls_ssl_free(&_ssl);
    mbedtls_pk_free(&_pkctx);

    set_own_cert(NULL);
    set_ca_chain(NULL);
    set_ssl_config(NULL);
}

void TLSSocketWrapper::set_hostname(const char *hostname)
{
    mbedtls_ssl_set_hostname(&_ssl, hostname);
}

nsapi_error_t TLSSocketWrapper::set_root_ca_cert(const void *root_ca, size_t len)
{
    mbedtls_x509_crt *crt;

    crt = new (std::nothrow) mbedtls_x509_crt;
    if (!crt) {
        return NSAPI_ERROR_NO_MEMORY;
    }

    mbedtls_x509_crt_init(crt);

    /* Parse CA certification */
    int ret;
    if ((ret = mbedtls_x509_crt_parse(crt, static_cast<const unsigned char *>(root_ca),
                        len)) != 0) {
        print_mbedtls_error("mbedtls_x509_crt_parse", ret);
        return NSAPI_ERROR_PARAMETER;
    }
    set_ca_chain(crt);
    _cacert_allocated = true;
    return NSAPI_ERROR_OK;
}

nsapi_error_t TLSSocketWrapper::set_root_ca_cert(const char *root_ca_pem)
{
    return set_root_ca_cert(root_ca_pem, strlen(root_ca_pem) + 1);
}

nsapi_error_t TLSSocketWrapper::set_client_cert_key(const char *client_cert_pem, const char *client_private_key_pem)
{
    return set_client_cert_key(client_cert_pem, strlen(client_cert_pem) + 1, client_private_key_pem, strlen(client_private_key_pem) + 1);
}

nsapi_error_t TLSSocketWrapper::set_client_cert_key(const void *client_cert, size_t client_cert_len,
        const void *client_private_key_pem, size_t client_private_key_len)
{
    int ret;
    mbedtls_x509_crt *crt = new mbedtls_x509_crt;
    mbedtls_x509_crt_init(crt);
    if((ret = mbedtls_x509_crt_parse(crt, static_cast<const unsigned char *>(client_cert),
            client_cert_len)) != 0) {
        print_mbedtls_error("mbedtls_x509_crt_parse", ret);
        return NSAPI_ERROR_PARAMETER;
    }
    mbedtls_pk_init(&_pkctx);
    if((ret = mbedtls_pk_parse_key(&_pkctx, static_cast<const unsigned char *>(client_private_key_pem),
            client_private_key_len, NULL, 0)) != 0) {
        print_mbedtls_error("mbedtls_pk_parse_key", ret);
        return NSAPI_ERROR_PARAMETER;
    }
    set_own_cert(crt);
    _clicert_allocated = true;

    return NSAPI_ERROR_OK;
}


nsapi_error_t TLSSocketWrapper::do_handshake() {
    nsapi_error_t _error;
    const char DRBG_PERS[] = "mbed TLS client";

    if (!_transport) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    _transport->set_blocking(true);
    /*
     * Initialize TLS-related stuf.
     */
    int ret;
    if ((ret = mbedtls_ctr_drbg_seed(&_ctr_drbg, mbedtls_entropy_func, &_entropy,
                        (const unsigned char *) DRBG_PERS,
                        sizeof (DRBG_PERS))) != 0) {
        print_mbedtls_error("mbedtls_crt_drbg_init", ret);
        _error = ret;
        return _error;
    }

    tr_info("mbedtls_ssl_conf_rng()");
    mbedtls_ssl_conf_rng(get_ssl_config(), mbedtls_ctr_drbg_random, &_ctr_drbg);


#if MBED_CONF_TLS_SOCKET_DEBUG_LEVEL > 0
    mbedtls_ssl_conf_verify(get_ssl_config(), my_verify, NULL);
    mbedtls_ssl_conf_dbg(get_ssl_config(), my_debug, NULL);
    mbedtls_debug_set_threshold(MBED_CONF_TLS_SOCKET_DEBUG_LEVEL);
#endif

    tr_info("mbedtls_ssl_setup()");
    if ((ret = mbedtls_ssl_setup(&_ssl, get_ssl_config())) != 0) {
        print_mbedtls_error("mbedtls_ssl_setup", ret);
        _error = ret;
        return _error;
    }

    mbedtls_ssl_set_bio(&_ssl, this, ssl_send, ssl_recv, NULL );

    /* Start the handshake, the rest will be done in onReceive() */
    tr_info("Starting TLS handshake with %s", _ssl.hostname);

    do {
        ret = mbedtls_ssl_handshake(&_ssl);
    } while (ret != 0 && (ret == MBEDTLS_ERR_SSL_WANT_READ ||
            ret == MBEDTLS_ERR_SSL_WANT_WRITE));
    if (ret < 0) {
        print_mbedtls_error("mbedtls_ssl_handshake", ret);
        return ret;
    }

    /* It also means the handshake is done, time to print info */
    tr_info("TLS connection to %s established\r\n", _ssl.hostname);

    /* Prints the server certificate and verify it. */
    const size_t buf_size = 1024;
    char* buf = new char[buf_size];
    mbedtls_x509_crt_info(buf, buf_size, "\r    ",
                    mbedtls_ssl_get_peer_cert(&_ssl));
    tr_debug("Server certificate:\r\n%s\r\n", buf);

    uint32_t flags = mbedtls_ssl_get_verify_result(&_ssl);
    if( flags != 0 ) {
        /* Verification failed. */
        mbedtls_x509_crt_verify_info(buf, buf_size, "\r  ! ", flags);
        tr_error("Certificate verification failed:\r\n%s", buf);
    } else {
        /* Verification succeeded. */
        tr_info("Certificate verification passed");
    }
    delete[] buf;

    _handshake_completed = true;

    return 0;
}


nsapi_error_t TLSSocketWrapper::send(const void *data, nsapi_size_t size) {
    int ret;

    if (!_transport) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    tr_debug("send %d", size);
    ret = mbedtls_ssl_write(&_ssl, (const unsigned char *) data, size);

    if (ret == MBEDTLS_ERR_SSL_WANT_WRITE ||
        ret == MBEDTLS_ERR_SSL_WANT_READ) {
        // translate to socket error
        return NSAPI_ERROR_WOULD_BLOCK;
    }
    if (ret < 0) {
        print_mbedtls_error("mbedtls_ssl_write", ret);
    }
    return ret; // Assume "non negative errorcode" to be propagated from Socket layer
}

nsapi_size_or_error_t TLSSocketWrapper::sendto(const SocketAddress &, const void *data, nsapi_size_t size)
{
    // Ignore the SocketAddress
    return send(data, size);
}

nsapi_size_or_error_t TLSSocketWrapper::recv(void *data, nsapi_size_t size) {
    int ret;

    if (!_transport) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    ret = mbedtls_ssl_read(&_ssl, (unsigned char *) data, size);

    if (ret == MBEDTLS_ERR_SSL_WANT_WRITE ||
        ret == MBEDTLS_ERR_SSL_WANT_READ) {
        // translate to socket error
        return NSAPI_ERROR_WOULD_BLOCK;
    } else if (ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY) {
        /* MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY is not considered as error.
         * Just ignre here. Once connection is closed, mbedtls_ssl_read()
         * will return 0.
         */
        return 0;
    } else if (ret < 0) {
        print_mbedtls_error("mbedtls_ssl_read", ret);
        // TODO: Should I translate SSL errors to some socket error?
    }
    return ret;
}

nsapi_size_or_error_t TLSSocketWrapper::recvfrom(SocketAddress *address, void *data, nsapi_size_t size)
{
    //TODO: Need Socket::getpeername() to get address
    return recv(data, size);
}

void TLSSocketWrapper::print_mbedtls_error(const char *name, int err) {
    char *buf = new char[128];
    mbedtls_strerror(err, buf, 128);
    tr_err("%s() failed: -0x%04x (%d): %s", name, -err, err, buf);
    delete[] buf;
}


#if MBED_CONF_TLS_SOCKET_DEBUG_LEVEL > 0

void TLSSocketWrapper::my_debug(void *ctx, int level, const char *file, int line,
                        const char *str)
{
    const char *p, *basename;
    (void) ctx;

    /* Extract basename from file */
    for(p = basename = file; *p != '\0'; p++) {
        if(*p == '/' || *p == '\\') {
            basename = p + 1;
        }
    }

    tr_debug("%s:%04d: |%d| %s", basename, line, level, str);
}


int TLSSocketWrapper::my_verify(void *data, mbedtls_x509_crt *crt, int depth, uint32_t *flags)
{
    const uint32_t buf_size = 1024;
    char *buf = new char[buf_size];
    (void) data;

    tr_debug("\nVerifying certificate at depth %d:\n", depth);
    mbedtls_x509_crt_info(buf, buf_size - 1, "  ", crt);
    tr_debug("%s", buf);

    if (*flags == 0)
        tr_info("No verification issue for this certificate\n");
    else
    {
        mbedtls_x509_crt_verify_info(buf, buf_size, "  ! ", *flags);
        tr_info("%s\n", buf);
    }

    delete[] buf;

    return 0;
}

#endif /* MBED_CONF_TLS_SOCKET_DEBUG_LEVEL > 0 */


int TLSSocketWrapper::ssl_recv(void *ctx, unsigned char *buf, size_t len) {
    int recv;

    TLSSocketWrapper *my = static_cast<TLSSocketWrapper *>(ctx);

    if (!my->_transport) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    recv = my->_transport->recv(buf, len);

    if (NSAPI_ERROR_WOULD_BLOCK == recv) {
        return MBEDTLS_ERR_SSL_WANT_READ;
    } else if(recv < 0) {
        tr_error("Socket recv error %d", recv);
    }
    // Propagate also Socket errors to SSL, it allows negative error codes to be returned here.
    return recv;
}

int TLSSocketWrapper::ssl_send(void *ctx, const unsigned char *buf, size_t len) {
    int size = -1;
    TLSSocketWrapper *my = static_cast<TLSSocketWrapper *>(ctx);

    if (!my->_transport) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    size = my->_transport->send(buf, len);

    if (NSAPI_ERROR_WOULD_BLOCK == size) {
        return MBEDTLS_ERR_SSL_WANT_WRITE;
    } else if(size < 0){
        tr_error("Socket send error %d", size);
    }
    // Propagate also Socket errors to SSL, it allows negative error codes to be returned here.
    return size;
 }


mbedtls_x509_crt *TLSSocketWrapper::get_own_cert()
{
    return _clicert;
}

int TLSSocketWrapper::set_own_cert(mbedtls_x509_crt *crt)
{
    int ret = 0;
    if (_clicert && _clicert_allocated) {
        mbedtls_x509_crt_free(_clicert);
        delete _clicert;
        _clicert_allocated = false;
    }
    _clicert = crt;
    if (crt) {
        if((ret = mbedtls_ssl_conf_own_cert(get_ssl_config(), _clicert, &_pkctx)) != 0) {
            print_mbedtls_error("mbedtls_ssl_conf_own_cert", ret);
        }
    }
    return ret;
}

mbedtls_x509_crt *TLSSocketWrapper::get_ca_chain()
{
    return _cacert;
}

void TLSSocketWrapper::set_ca_chain(mbedtls_x509_crt *crt)
{
    if (_cacert && _cacert_allocated) {
        mbedtls_x509_crt_free(_cacert);
        delete _cacert;
        _cacert_allocated = false;
    }
    _cacert = crt;
    tr_info("mbedtls_ssl_conf_ca_chain()");
    mbedtls_ssl_conf_ca_chain(get_ssl_config(), _cacert, NULL);
}

mbedtls_ssl_config *TLSSocketWrapper::get_ssl_config()
{
    if (!_ssl_conf) {
        int ret;
        _ssl_conf = new mbedtls_ssl_config;
        mbedtls_ssl_config_init(_ssl_conf);
        _ssl_conf_allocated = true;

        tr_info("mbedtls_ssl_config_defaults()");
        if ((ret = mbedtls_ssl_config_defaults(_ssl_conf,
                        MBEDTLS_SSL_IS_CLIENT,
                        MBEDTLS_SSL_TRANSPORT_STREAM,
                        MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
            print_mbedtls_error("mbedtls_ssl_config_defaults", ret);
            set_ssl_config(NULL);
            //TODO: Change to MBED_ERROR
            return NULL;
        }
        /* It is possible to disable authentication by passing
         * MBEDTLS_SSL_VERIFY_NONE in the call to mbedtls_ssl_conf_authmode()
         */
        tr_info("mbedtls_ssl_conf_authmode()");
        mbedtls_ssl_conf_authmode(get_ssl_config(), MBEDTLS_SSL_VERIFY_REQUIRED);
    }
    return _ssl_conf;
}

void TLSSocketWrapper::set_ssl_config(mbedtls_ssl_config *conf)
{
    if (_ssl_conf && _ssl_conf_allocated) {
        mbedtls_ssl_config_free(_ssl_conf);
        delete _ssl_conf;
        _ssl_conf_allocated = false;
    }
    _ssl_conf = conf;
}

nsapi_error_t TLSSocketWrapper::close()
{
    if (!_transport) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    tr_info("Closing TLS");

    int ret = 0;
    if (_handshake_completed) {
        _transport->set_blocking(true);
        ret = mbedtls_ssl_close_notify(&_ssl);
        if (ret) {
            print_mbedtls_error("mbedtls_ssl_close_notify", ret);
        }
        _handshake_completed = false;
    }

    if (_close_transport) {
        int ret2 = _transport->close();
        if (!ret) {
            ret = ret2;
        }
    }

    _transport = NULL;

    return ret;
}

nsapi_error_t TLSSocketWrapper::connect(const SocketAddress &address)
{
    if (!_transport) {
        return NSAPI_ERROR_NO_SOCKET;
    }

    if (_connect_transport) {
        nsapi_error_t ret = _transport->connect(address);
        if (ret) {
            return ret;
        }
    }
    return do_handshake();
}

nsapi_error_t TLSSocketWrapper::bind(const SocketAddress &address)
{
    if (!_transport) {
        return NSAPI_ERROR_NO_SOCKET;
    }
    return _transport->bind(address);
}

void TLSSocketWrapper::set_blocking(bool blocking)
{
    if (!_transport) {
        return;
    }
    _transport->set_blocking(blocking);
}

void TLSSocketWrapper::set_timeout(int timeout)
{
    if (!_transport) {
        return;
    }
    _transport->set_timeout(timeout);
}

void TLSSocketWrapper::sigio(mbed::Callback<void()> func)
{
    if (!_transport) {
        return;
    }
    // Allow sigio() to propagate to upper level and handle errors on recv() and send()
    _transport->sigio(func);
}

nsapi_error_t TLSSocketWrapper::setsockopt(int level, int optname, const void *optval, unsigned optlen)
{
    if (!_transport) {
        return NSAPI_ERROR_NO_SOCKET;
    }
    return _transport->setsockopt(level, optname, optval, optlen);
}

nsapi_error_t TLSSocketWrapper::getsockopt(int level, int optname, void *optval, unsigned *optlen)
{
    if (!_transport) {
        return NSAPI_ERROR_NO_SOCKET;
    }
    return _transport->getsockopt(level, optname, optval, optlen);
}

Socket *TLSSocketWrapper::accept(nsapi_error_t *err)
{
    if (err) {
        *err = NSAPI_ERROR_UNSUPPORTED;
    }
    return NULL;
}

nsapi_error_t TLSSocketWrapper::listen(int)
{
    return NSAPI_ERROR_UNSUPPORTED;
}
