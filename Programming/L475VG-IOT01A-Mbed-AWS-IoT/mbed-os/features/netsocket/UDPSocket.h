
/** \addtogroup netsocket */
/** @{*/
/* UDPSocket
 * Copyright (c) 2015 ARM Limited
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

#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "netsocket/InternetSocket.h"
#include "netsocket/NetworkStack.h"
#include "netsocket/NetworkInterface.h"
#include "rtos/EventFlags.h"


/** UDP socket
 */
class UDPSocket : public InternetSocket {
public:
    /** Create an uninitialized socket
     *
     *  Must call open to initialize the socket on a network stack.
     */
    UDPSocket();

    /** Create a socket on a network interface
     *
     *  Creates and opens a socket on the network stack of the given
     *  network interface.
     *
     *  @param stack    Network stack as target for socket
     */
    template <typename S>
    UDPSocket(S *stack)
    {
        open(stack);
    }

    /** Destroy a socket
     *
     *  Closes socket if the socket is still open
     */
    virtual ~UDPSocket();

    /** Send a packet over a UDP socket
     *
     *  Sends data to the specified address specified by either a domain name
     *  or an IP address and port. Returns the number of bytes sent from the
     *  buffer.
     *
     *  By default, sendto blocks until data is sent. If socket is set to
     *  non-blocking or times out, NSAPI_ERROR_WOULD_BLOCK is returned
     *  immediately.
     *
     *  @param host     Hostname of the remote host
     *  @param port     Port of the remote host
     *  @param data     Buffer of data to send to the host
     *  @param size     Size of the buffer in bytes
     *  @return         Number of sent bytes on success, negative error
     *                  code on failure
     */
    virtual nsapi_size_or_error_t sendto(const char *host, uint16_t port,
                                         const void *data, nsapi_size_t size);

    /** Send a packet over a UDP socket
     *
     *  Sends data to the specified address. Returns the number of bytes
     *  sent from the buffer.
     *
     *  By default, sendto blocks until data is sent. If socket is set to
     *  non-blocking or times out, NSAPI_ERROR_WOULD_BLOCK is returned
     *  immediately.
     *
     *  @param address  The SocketAddress of the remote host
     *  @param data     Buffer of data to send to the host
     *  @param size     Size of the buffer in bytes
     *  @return         Number of sent bytes on success, negative error
     *                  code on failure
     */
    virtual nsapi_size_or_error_t sendto(const SocketAddress &address,
                                         const void *data, nsapi_size_t size);

    /** Receive a datagram over a UDP socket
     *
     *  Receives a datagram and stores the source address in address if address
     *  is not NULL. Returns the number of bytes written into the buffer. If the
     *  datagram is larger than the buffer, the excess data is silently discarded.
     *
     *  If socket is connected, only packets coming from connected peer address
     *  are accepted.
     *
     *  @note recvfrom() is allowed write to address and data buffers even if error occurs.
     *
     *  By default, recvfrom blocks until a datagram is received. If socket is set to
     *  non-blocking or times out with no datagram, NSAPI_ERROR_WOULD_BLOCK
     *  is returned.
     *
     *  @param address  Destination for the source address or NULL
     *  @param data     Destination buffer for datagram received from the host
     *  @param size     Size of the buffer in bytes
     *  @return         Number of received bytes on success, negative error
     *                  code on failure
     */
    virtual nsapi_size_or_error_t recvfrom(SocketAddress *address,
                                           void *data, nsapi_size_t size);

    /** Set remote peer address
     *
     *  Set the remote address for next send() call and filtering
     *  for incomming packets. To reset the address, zero initialised
     *  SocketAddress must be in the address parameter.
     *
     *  @param address  The SocketAddress of the remote host
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t connect(const SocketAddress &address);

    /** Send a datagram to pre-specified remote.
     *
     *  The socket must be connected to a remote host before send() call.
     *  Returns the number of bytes sent from the buffer.
     *
     *  By default, send blocks until all data is sent. If socket is set to
     *  non-blocking or times out, a partial amount can be written.
     *  NSAPI_ERROR_WOULD_BLOCK is returned if no data was written.
     *
     *  @param data     Buffer of data to send to the host
     *  @param size     Size of the buffer in bytes
     *  @return         Number of sent bytes on success, negative error
     *                  code on failure.
     */
    virtual nsapi_size_or_error_t send(const void *data, nsapi_size_t size);

    /** Receive data from a socket.
     *
     *  This is equivalent of calling recvfrom(NULL, data, size).
     *
     *  If socket is connected, only packets coming from connected peer address
     *  are accepted.
     *
     *  @note recv() is allowed write to data buffer even if error occurs.
     *
     *  By default, recv blocks until some data is received. If socket is set to
     *  non-blocking or times out, NSAPI_ERROR_WOULD_BLOCK can be returned to
     *  indicate no data.
     *
     *  @param data     Destination buffer for data received from the host
     *  @param size     Size of the buffer in bytes
     *  @return         Number of received bytes on success, negative error
     *                  code on failure.
     */
    virtual nsapi_size_or_error_t recv(void *data, nsapi_size_t size);

    /** Not implemented for UDP
     *
     *  @param error      unused
     *  @return           NSAPI_ERROR_UNSUPPORTED
     */
    virtual Socket *accept(nsapi_error_t *error = NULL);

    /** Not implemented for UDP
     *
     *  @param backlog    unused
     *  @return           NSAPI_ERROR_UNSUPPORTED
     */
    virtual nsapi_error_t listen(int backlog = 1);

protected:
    virtual nsapi_protocol_t get_proto();
};


#endif

/** @}*/
