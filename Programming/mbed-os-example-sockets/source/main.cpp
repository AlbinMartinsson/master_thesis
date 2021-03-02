/* Sockets Example
 * Copyright (c) 2016-2020 ARM Limited
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

#include "mbed.h"
#include "wifi_helper.h"
#include "mbed-trace/mbed_trace.h"
#include "MbedJSONValue.h"
#include <string>
#include "stm32l475e_iot01_tsensor.h"
#include "config.h"
#include "http_request.h"

#if MBED_CONF_APP_USE_TLS_SOCKET
#include "root_ca_cert.h"

#ifndef DEVICE_TRNG
#error "mbed-os-example-tls-socket requires a device which supports TRNG"
#endif
#endif // MBED_CONF_APP_USE_TLS_SOCKET

class SocketDemo
{
    static constexpr size_t MAX_NUMBER_OF_ACCESS_POINTS = 10;
    static constexpr size_t MAX_MESSAGE_RECEIVED_LENGTH = 100;

#if MBED_CONF_APP_USE_TLS_SOCKET
    static constexpr size_t REMOTE_PORT = 443; // tls port
#else
    static constexpr size_t SERVICE_REGISRTY_PORT = 8443; // standard HTTP port
    static constexpr size_t AUTHORIZATION_PORT = 8445;    // standard HTTP port
    static constexpr size_t ORCHESTRATOR_PORT = 8441;     // standard HTTP port
    static constexpr size_t PROVIDER_PORT = 1234;         // standard HTTP port
#endif // MBED_CONF_APP_USE_TLS_SOCKET

public:
    SocketDemo() : _net(NetworkInterface::get_default_instance())
    {
    }

    ~SocketDemo()
    {
        if (_net)
        {
            _net->disconnect();
        }
    }

    void run()
    {

        /* Inits and reads the temperature from the sensor.
            TODO send as float instead of int
        */

        BSP_TSENSOR_Init();
        int temperature = 0;

        temperature = BSP_TSENSOR_ReadTemp();

        if (!_net)
        {
            printf("Error! No network interface found.\r\n");
            return;
        }

        /* if we're using a wifi interface run a quick scan */
        if (_net->wifiInterface())
        {
            /* the scan is not required to connect and only serves to show visible access points */
            wifi_scan();

            /* in this example we use credentials configured at compile time which are used by
             * NetworkInterface::connect() but it's possible to do this at runtime by using the
             * WiFiInterface::connect() which takes these parameters as arguments */
        }

        /* connect will perform the action appropriate to the interface type to connect to the network */

        printf("Connecting to the network...\r\n");

        nsapi_size_or_error_t result = _net->connect();
        if (result != 0)
        {
            printf("Error! _net->connect() returned: %d\r\n", result);
            return;
        }

        print_network_info();

        /* opening the socket only allocates resources */

        result = _socket.open(_net);
        if (result != 0)
        {
            printf("Error! _socket.open() returned: %d\r\n", result);
            return;
        }

        /*
        Registers a provider 
        TODO Not getting status code, see what is up with that.
        */

        std::string sr_register_system_url = "http://localhost:8443/serviceregistry/mgmt/systems";
        std::string register_provider_body = "{\"address\": \"localhost\", \"authenticationInfo\": \"\", \"port\": 1234, \"systemName\": \"test_provider\"}";
        std::string register_provider_response = http_post_request_with_response(_net, sr_register_system_url, register_provider_body);
        printf("%s\n", register_provider_response.c_str());
        int provider_id = find_id(register_provider_response.c_str());

        /*
        Registers a consumer 
        TODO Not getting status code, see what is up with that.
        */

        std::string register_consumer_body = "{\"address\": \"localhost\", \"authenticationInfo\": \"\", \"port\": 1234, \"systemName\": \"test_consumer\"}";
        std::string register_consumer_response = http_post_request_with_response(_net, sr_register_system_url, register_consumer_body);
        printf("%s\n", register_consumer_response.c_str());
        int consumer_id = find_id(register_consumer_response.c_str());

        /*
        Registers a service definition
        TODO Not getting status code, see what is up with that.
        */

        std::string sr_register_service_url = "http://localhost:8443/serviceregistry/mgmt";
        std::string register_service_body = "{\"serviceDefinition\": \"test_service_definition1\", \"providerSystem\": {\"systemName\": \"test_provider\", \"address\": \"localhost\", \"port\": 1234, \"authenticationInfo\": \"\" }, \"interfaces\": [\"HTTPS-SECURE-JSON\"]}\r\n";
        std::string register_service_response = http_post_request_with_response(_net, sr_register_service_url, register_service_body);
        printf("%s\n", register_service_response.c_str());
        int service_id = find_service_id(register_service_response.c_str());

        /*
        Performs intracloud authorization
        TODO Not getting status code, see what is up with that.
        */

        std::string authorization_url = "http://localhost:8445/authorization/mgmt/intracloud";
        std::string add_intracloud_authorization_body = "{\"consumerId\": " + std::to_string(consumer_id) + ",\"interfaceIds\": [3], \"providerIds\": [" + std::to_string(provider_id) + "], \"serviceDefinitionIds\": [" + std::to_string(service_id) + "]}\r\n";
        std::string add_intracloud_authorization_response = http_post_request_with_response(_net, authorization_url, add_intracloud_authorization_body);
        printf("%s\n", add_intracloud_authorization_response.c_str());

        /*
        Creates a orcestrator store entry
        TODO Not getting status code, see what is up with that.
        */

        std::string orchestration_store_url = "http://localhost:8441/orchestrator/mgmt/store";
        std::string create_orchestration_store_body = "[{ \"serviceDefinitionName\": \"test_service_definition1\", \"consumerSystemId\": " + std::to_string(consumer_id) + ", \"providerSystem\": { \"systemName\":  \"test_provider\", \"address\": \"localhost\", \"port\": 1234, \"authenticationInfo\": \"\"}, \"cloud\": { \"operator\": \"aitia\", \"name\": \"testcloud2\" }, \"serviceInterfaceName\": \"HTTPS-SECURE-JSON\", \"priority\": 1}]\r\n";
        std::string create_orchestration_store_response = http_post_request_with_response(_net, orchestration_store_url, create_orchestration_store_body);
        printf("%s\n", create_orchestration_store_response.c_str());

        /*
        Starts orchestration based on ID.
        TODO Not getting status code, see what is up with that.
        */

        std::string orchestration_based_on_id_url = "http://localhost:8441/orchestrator/orchestration/" + std::to_string(consumer_id);
        std::string start_orchestration_based_on_id_response = http_get_request_with_response(_net, orchestration_based_on_id_url);
        printf("%s\n", start_orchestration_based_on_id_response.c_str());

        /*
        Post temperature to provider. Optional part, requires a flask app or similar running as a provider to serve the request.
        */
        std::string provider_url = find_address_and_port(start_orchestration_based_on_id_response) + "temperature";
        std::string provider_body = "{\"temperature\": \"" + std::to_string(temperature) + "\"}\r\n";
        std::string provider_response = http_post_request_with_response(_net, provider_url, provider_body);
        printf("%s\n", provider_response.c_str());

#if MBED_CONF_APP_USE_TLS_SOCKET
        result = _socket.set_root_ca_cert(root_ca_cert);
        if (result != NSAPI_ERROR_OK)
        {
            printf("Error: _socket.set_root_ca_cert() returned %d\n", result);
            return;
        }
        _socket.set_hostname(MBED_CONF_APP_HOSTNAME);
#endif // MBED_CONF_APP_USE_TLS_SOCKET

        printf("Demo concluded successfully \r\n");
    }

    void wifi_scan()
    {
        WiFiInterface *wifi = _net->wifiInterface();

        WiFiAccessPoint ap[MAX_NUMBER_OF_ACCESS_POINTS];

        /* scan call returns number of access points found */
        int result = wifi->scan(ap, MAX_NUMBER_OF_ACCESS_POINTS);

        if (result <= 0)
        {
            printf("WiFiInterface::scan() failed with return value: %d\r\n", result);
            return;
        }

        printf("%d networks available:\r\n", result);

        for (int i = 0; i < result; i++)
        {
            printf("Network: %s secured: %s BSSID: %hhX:%hhX:%hhX:%hhx:%hhx:%hhx RSSI: %hhd Ch: %hhd\r\n",
                   ap[i].get_ssid(), get_security_string(ap[i].get_security()),
                   ap[i].get_bssid()[0], ap[i].get_bssid()[1], ap[i].get_bssid()[2],
                   ap[i].get_bssid()[3], ap[i].get_bssid()[4], ap[i].get_bssid()[5],
                   ap[i].get_rssi(), ap[i].get_channel());
        }
        printf("\r\n");
    }

    void print_network_info()
    {
        /* print the network info */
        SocketAddress a;
        _net->get_ip_address(&a);
        printf("IP address: %s\r\n", a.get_ip_address() ? a.get_ip_address() : "None");
        _net->get_netmask(&a);
        printf("Netmask: %s\r\n", a.get_ip_address() ? a.get_ip_address() : "None");
        _net->get_gateway(&a);
        printf("Gateway: %s\r\n", a.get_ip_address() ? a.get_ip_address() : "None");
    }

    void dump_response(HttpResponse *res)
    {
        printf("Status: %d - %s\n", res->get_status_code(), res->get_status_message().c_str());

        printf("Headers:\n");
        for (size_t ix = 0; ix < res->get_headers_length(); ix++)
        {
            printf("\t%s: %s\n", res->get_headers_fields()[ix]->c_str(), res->get_headers_values()[ix]->c_str());
        }
        printf("\nBody (%d bytes):\n\n%s\n", res->get_body_length(), res->get_body_as_string().c_str());
    }

    /*
    Finds the id of the systems.
    TODO rewrite to match find_address_and_port? 
    */

    int find_id(const char *str)
    {
        const char *pch = strstr(str, "\"id\":");
        if (pch)
        {
            int total_n = 0;
            int n;
            int i;
            while (1 == sscanf(pch + total_n, "%*[^0123456789]%d%n", &i, &n))
            {
                total_n += n;
                return i;
            }
        }
        return 0;
    }

    /*
    Finds the id of the services.
    TODO rewrite to match find_address_and_port? 
    */

    int find_service_id(const char *str)
    {
        const char *pch = strstr(str, "\"serviceDefinition\":{\"id\":");
        if (pch)
        {
            int total_n = 0;
            int n;
            int i;
            while (1 == sscanf(pch + total_n, "%*[^0123456789]%d%n", &i, &n))
            {
                total_n += n;
                return i;
            }
        }
        return 0;
    }

    /*
    Finds the address and port of the provider.
    TODO rewrite to match find_address_and_port? 
    */

    std::string find_address_and_port(std::string str)
    {
        std::string before_address = "\"address\":\"";
        std::size_t pos_before_address = str.find(before_address);
        std::string before_port = "\",\"port\":";
        std::size_t pos_before_port = str.find("\",\"port\":");
        std::string address = str.substr(pos_before_address + before_address.length(), pos_before_port - pos_before_address - before_address.length());
        std::string after_port = ",\"authenticationInfo\"";
        std::size_t pos_after_port = str.find(after_port);
        std::string port = str.substr(pos_before_port + before_port.length(), pos_after_port - pos_before_port - before_port.length());
        return "http://" + address + ":" + port + "/";
    }

    /*
    Makes a HTTP POST to url with body
    */

    std::string http_post_request_with_response(NetworkInterface *_net, std::string url, std::string body)
    {
        HttpRequest *post_request = new HttpRequest(_net, HTTP_POST, url.c_str());
        post_request->set_header("Content-Type", "application/json");
        HttpResponse *post_response = post_request->send(body.c_str(), strlen(body.c_str()));
        if (!post_response)
        {
            printf("HttpRequest failed (error code %d)\n", post_request->get_error());
            return std::to_string(post_request->get_error());
        }
        printf("\n----- HTTP POST response -----\n");
        //dump_response(post_response);
        std::string response_body = post_response->get_body_as_string();
        delete post_response;
        return response_body;
    }

    /*
    Makes a HTTP GET to url with body
    */

    std::string http_get_request_with_response(NetworkInterface *_net, std::string url)
    {
        HttpRequest *get_request = new HttpRequest(_net, HTTP_GET, url.c_str());

        HttpResponse *get_request_response = get_request->send();

        if (!get_request_response)
        {
            printf("HttpRequest failed (error code %d)\n", get_request->get_error());
            return std::to_string(get_request->get_error());
        }
        printf("\n----- HTTP GET response -----\n");
        //dump_response(get_request_response);
        std::string response_body = get_request_response->get_body_as_string();
        delete get_request_response;
        return response_body;
    }

private:
    NetworkInterface *_net;

#if MBED_CONF_APP_USE_TLS_SOCKET
    TLSSocket _socket;
#else
    TCPSocket _socket;
#endif // MBED_CONF_APP_USE_TLS_SOCKET
};

int main()
{
    printf("\r\nStarting arrowhead demo\r\n\r\n");

#ifdef MBED_CONF_MBED_TRACE_ENABLE
    mbed_trace_init();
#endif

    SocketDemo *example = new SocketDemo();
    MBED_ASSERT(example);
    example->run();

    return 0;
}
