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

        std::string base_sr_url = "http://localhost:8443";

        HttpRequest *reg_provider_req = new HttpRequest(_net, HTTP_POST, (base_sr_url + "/serviceregistry/mgmt/systems").c_str());
        reg_provider_req->set_header("Content-Type", "application/json");

        const char register_provider[] = "{\"address\": \"localhost\", \"authenticationInfo\": \"\", \"port\": 1234, \"systemName\": \"test_provider\"}";

        HttpResponse *reg_provider_res = reg_provider_req->send(register_provider, strlen(register_provider));
        if (!reg_provider_res)
        {
            printf("HttpRequest failed (error code %d)\n", reg_provider_req->get_error());
            return;
        }

        printf("\n----- HTTP POST response -----\n");
        dump_response(reg_provider_res);
        printf("%d\n", find_id(reg_provider_res->get_body_as_string().c_str()));
        int provider_id = find_id(reg_provider_res->get_body_as_string().c_str());
        delete reg_provider_req;

        /*
        Registers a consumer 
        TODO Not getting status code, see what is up with that.
        */

        HttpRequest *reg_consumer_req = new HttpRequest(_net, HTTP_POST, (base_sr_url + "/serviceregistry/mgmt/systems").c_str());
        reg_consumer_req->set_header("Content-Type", "application/json");

        const char register_consumer[] = "{\"address\": \"localhost\", \"authenticationInfo\": \"\", \"port\": 1256, \"systemName\": \"test_consumer\"}";

        HttpResponse *reg_consumer_res = reg_consumer_req->send(register_consumer, strlen(register_consumer));
        if (!reg_consumer_res)
        {
            printf("HttpRequest failed (error code %d)\n", reg_consumer_req->get_error());
            return;
        }

        printf("\n----- HTTP POST response -----\n");
        dump_response(reg_consumer_res);
        printf("%d\n", find_id(reg_consumer_res->get_body_as_string().c_str()));
        int consumer_id = find_id(reg_consumer_res->get_body_as_string().c_str());
        delete reg_consumer_req;

        /*
        Registers a service definition
        TODO Not getting status code, see what is up with that.
        */

        HttpRequest *reg_service_definition_req = new HttpRequest(_net, HTTP_POST, (base_sr_url + "/serviceregistry/mgmt").c_str());
        reg_service_definition_req->set_header("Content-Type", "application/json");

        const char register_service_definition[] = "{\"serviceDefinition\": \"test_service_definition\", \"providerSystem\": {\"systemName\": \"test_provider\", \"address\": \"localhost\", \"port\": 1234, \"authenticationInfo\": \"\" }, \"interfaces\": [\"HTTPS-SECURE-JSON\"]}\r\n";

        HttpResponse *reg_service_definition_res = reg_service_definition_req->send(register_service_definition, strlen(register_service_definition));
        if (!reg_service_definition_res)
        {
            printf("HttpRequest failed (error code %d)\n", reg_service_definition_req->get_error());
            return;
        }

        printf("\n----- HTTP POST response -----\n");
        dump_response(reg_service_definition_res);
        printf("%d\n", find_id(reg_service_definition_res->get_body_as_string().c_str()));
        int service_definition_id = find_service_id(reg_service_definition_res->get_body_as_string().c_str());
        delete reg_service_definition_req;

        /*
        Performs intracloud authorization
        TODO Not getting status code, see what is up with that.
        */

        std::string base_auth_url = "http://localhost:8445";
        std::string add_intracloud_authorization = "{\"consumerId\": " + std::to_string(consumer_id) + ",\"interfaceIds\": [3], \"providerIds\": [" + std::to_string(provider_id) + "], \"serviceDefinitionIds\": [" + std::to_string(service_definition_id) + "]}\r\n";

        HttpRequest *add_intracloud_authorization_req = new HttpRequest(_net, HTTP_POST, (base_auth_url + "/authorization/mgmt/intracloud").c_str());
        add_intracloud_authorization_req->set_header("Content-Type", "application/json");

        HttpResponse *add_intracloud_authorization_res = add_intracloud_authorization_req->send(add_intracloud_authorization.c_str(), strlen(add_intracloud_authorization.c_str()));
        if (!add_intracloud_authorization_res)
        {
            printf("HttpRequest failed (error code %d)\n", add_intracloud_authorization_req->get_error());
            return;
        }

        printf("\n----- HTTP POST response -----\n");
        dump_response(add_intracloud_authorization_res);
        delete add_intracloud_authorization_res;

        /*
        Creates a orcestrator store entry
        TODO Not getting status code, see what is up with that.
        */

        std::string base_orch_url = "http://localhost:8441";
        std::string create_orchestration_store = "[{ \"serviceDefinitionName\": \"test_service_definition\", \"consumerSystemId\": " + std::to_string(consumer_id) + ", \"providerSystem\": { \"systemName\":  \"test_provider\", \"address\": \"localhost\", \"port\": 1234, \"authenticationInfo\": \"\"}, \"cloud\": { \"operator\": \"aitia\", \"name\": \"testcloud2\" }, \"serviceInterfaceName\": \"HTTPS-SECURE-JSON\", \"priority\": 1}]\r\n";
        HttpRequest *create_orchestration_store_req = new HttpRequest(_net, HTTP_POST, (base_orch_url + "/orchestrator/mgmt/store").c_str());
        create_orchestration_store_req->set_header("Content-Type", "application/json");

        HttpResponse *create_orchestration_store_res = create_orchestration_store_req->send(create_orchestration_store.c_str(), strlen(create_orchestration_store.c_str()));
        if (!create_orchestration_store_res)
        {
            printf("HttpRequest failed (error code %d)\n", create_orchestration_rule_req->get_error());
            return;
        }

        printf("\n----- HTTP POST response -----\n");
        dump_response(create_orchestration_store_res);
        delete create_orchestration_store_res;

        /*
        Starts orchestration based on ID.
        TODO Not getting status code, see what is up with that.
        */
        
        HttpRequest *start_orchestration_based_on_id_req = new HttpRequest(_net, HTTP_GET, (base_orch_url + "/orchestrator/orchestration/" + std::to_string(consumer_id)).c_str());
        start_orchestration_based_on_id_req->set_header("Content-Type", "application/json");

        HttpResponse *start_orchestration_based_on_id_res = start_orchestration_based_on_id_req->send();

        if (!start_orchestration_based_on_id_res)
        {
            printf("HttpRequest failed (error code %d)\n", start_orchestration_based_on_id_req->get_error());
            return;
        }

        dump_response(start_orchestration_based_on_id_res);
        std::string provider_url = find_address_and_port(start_orchestration_based_on_id_res->get_body_as_string());
        delete start_orchestration_based_on_id_res;

        /*
        Post temperature to local provider. Getting HHTP status here so something "wrong" with arrowhead.
        */

        std::string temperature_json = "{\"temperature\": " + std::to_string(temperature) + "}";
        printf("%s\n", temperature_json.c_str());
        printf("%s\n", provider_url.c_str());
        HttpRequest *post_temperature_req = new HttpRequest(_net, HTTP_POST, (provider_url + "/temperature").c_str());
        post_temperature_req->set_header("Content-Type", "application/json");

        HttpResponse *post_temperature_res = post_temperature_req->send(temperature_json.c_str(), strlen(temperature_json.c_str()));
        if (!add_intracloud_authorization_res)
        {
            printf("HttpRequest failed (error code %d)\n", post_temperature_req->get_error());
            return;
        }

        printf("\n----- HTTP POST response -----\n");
        dump_response(post_temperature_res);
        delete post_temperature_res;

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
        printf("%s\n", pch);
        if (pch)
        {
            int total_n = 0;
            int n;
            int i;
            while (1 == sscanf(pch + total_n, "%*[^0123456789]%d%n", &i, &n))
            {
                total_n += n;
                printf("%d\n", i);
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
        printf("%s\n", pch);
        if (pch)
        {
            int total_n = 0;
            int n;
            int i;
            while (1 == sscanf(pch + total_n, "%*[^0123456789]%d%n", &i, &n))
            {
                total_n += n;
                printf("%d\n", i);
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
        return "http://" + address + ":" + port;
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
