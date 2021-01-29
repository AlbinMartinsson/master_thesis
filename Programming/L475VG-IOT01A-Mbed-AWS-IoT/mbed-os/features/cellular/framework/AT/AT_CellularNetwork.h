/*
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
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

#ifndef AT_CELLULAR_NETWORK_H_
#define AT_CELLULAR_NETWORK_H_

#include "CellularNetwork.h"
#include "AT_CellularBase.h"
#include "NetworkStack.h"

namespace mbed {

#define AT_NETWORK_TRIALS 5

/**
 *  Class AT_CellularNetwork
 *
 *  Class for connecting to a network and getting information from it.
 */
class AT_CellularNetwork : public CellularNetwork, public AT_CellularBase {

public:

    AT_CellularNetwork(ATHandler &atHandler);
    virtual ~AT_CellularNetwork();
    // declare friend so it can access stack
    friend class AT_CellularDevice;

public: // NetworkInterface

    virtual nsapi_error_t set_credentials(const char *apn,
                                          const char *username = 0, const char *password = 0);

    virtual nsapi_error_t set_credentials(const char *apn, AuthenticationType type,
                                          const char *username = 0, const char *password = 0);

    virtual nsapi_error_t connect(const char *apn,
                                  const char *username = 0, const char *password = 0);

    virtual nsapi_error_t connect();

    virtual nsapi_error_t disconnect();

protected:
    virtual NetworkStack *get_stack();

public: // CellularNetwork
    virtual nsapi_error_t init();

    virtual nsapi_error_t activate_context();

    virtual nsapi_error_t set_registration(const char *plmn = 0);

    virtual nsapi_error_t get_network_registering_mode(NWRegisteringMode &mode);

    virtual nsapi_error_t get_registration_status(RegistrationType type, RegistrationStatus &status);

    virtual nsapi_error_t set_attach(int timeout = 10 * 1000);

    virtual nsapi_error_t get_attach(AttachStatus &status);

    virtual nsapi_error_t detach();

    virtual nsapi_error_t get_rate_control(CellularNetwork::RateControlExceptionReports &reports,
                                           CellularNetwork::RateControlUplinkTimeUnit &time_unit, int &uplink_rate);

    virtual nsapi_error_t get_apn_backoff_timer(int &backoff_timer);

    virtual void attach(Callback<void(nsapi_event_t, intptr_t)> status_cb);

    virtual nsapi_connection_status_t get_connection_status() const;

    virtual nsapi_error_t set_blocking(bool blocking);

    virtual const char *get_ip_address();

    virtual nsapi_error_t set_access_technology(RadioAccessTechnology rat);
    virtual nsapi_error_t get_access_technology(RadioAccessTechnology &rat);

    virtual nsapi_error_t scan_plmn(operList_t &operators, int &ops_count);

    virtual nsapi_error_t set_ciot_optimization_config(Supported_UE_Opt supported_opt,
                                                       Preferred_UE_Opt preferred_opt);

    virtual nsapi_error_t get_ciot_optimization_config(Supported_UE_Opt &supported_opt,
                                                       Preferred_UE_Opt &preferred_opt);

    virtual nsapi_error_t set_stack_type(nsapi_ip_stack_t stack_type);

    virtual nsapi_ip_stack_t get_stack_type();

    virtual nsapi_error_t get_pdpcontext_params(pdpContextList_t &params_list);

    virtual nsapi_error_t get_extended_signal_quality(int &rxlev, int &ber, int &rscp, int &ecno, int &rsrq, int &rsrp);

    virtual nsapi_error_t get_signal_quality(int &rssi, int &ber);

    virtual nsapi_error_t get_cell_id(int &cell_id);

    virtual int get_3gpp_error();

    virtual nsapi_error_t get_operator_params(int &format, operator_t &operator_params);

    virtual nsapi_error_t set_registration_urc(RegistrationType type, bool on);

    virtual nsapi_error_t get_operator_names(operator_names_list &op_names);
protected:

    /** Check if modem supports the given stack type.
     *
     *  @return true if supported
     */
    virtual bool get_modem_stack_type(nsapi_ip_stack_t requested_stack);

    /** Check if modem supports given registration type.
     *
     *  @param reg_type enum RegistrationType
     *  @return         mode supported on given reg_type as per 3GPP TS 27.007, 0 when unsupported
     */
    enum RegistrationMode {
        RegistrationModeDisable = 0,
        RegistrationModeEnable, // <stat>
        RegistrationModeLAC, // <stat>[,<[lac>,]<[ci>],[<AcT>],[<rac>]]
    };
    virtual RegistrationMode has_registration(RegistrationType reg_type);

    /** Sets access technology to be scanned. Modem specific implementation.
     *
     *  @param op_rat Access technology
     *
     *  @return       zero on success
     */
    virtual nsapi_error_t set_access_technology_impl(RadioAccessTechnology op_rat);

    /** APN user authentication
     *
     *  @return       NSAPI_ERROR_OK on success
     *                NSAPI_ERROR_UNSUPPORTED on authentication not supported by cellular device
     *                NSAPI_ERROR_AUTH_FAILURE on authentication to network failed
     */
    virtual nsapi_error_t do_user_authentication();
private:
    //  "NO CARRIER" urc
    void urc_no_carrier();
    void urc_creg();
    void urc_cereg();
    void urc_cgreg();
    void urc_cgev();

    nsapi_ip_stack_t string_to_stack_type(const char *pdp_type);

    void free_credentials();

    nsapi_error_t open_data_channel();
    bool get_context();
    bool set_new_context(int cid);

    nsapi_error_t delete_current_context();

    void read_reg_params_and_compare(RegistrationType type);
    void read_reg_params(RegistrationType type, RegistrationStatus &reg_status, int &lac, int &cell_id, int &act);
    // calls network callback only if status was changed, updates local connection status
    void call_network_cb(nsapi_connection_status_t status);
#if NSAPI_PPP_AVAILABLE
    void ppp_status_cb(nsapi_event_t, intptr_t);
#endif

protected:
    NetworkStack *_stack;
    char *_apn;
    char *_uname;
    char *_pwd;
    nsapi_ip_stack_t _ip_stack_type_requested;
    nsapi_ip_stack_t _ip_stack_type;
    int _cid;
    Callback<void(nsapi_event_t, intptr_t)> _connection_status_cb;
    RadioAccessTechnology _op_act;
    AuthenticationType _authentication_type;
    int _cell_id;
    nsapi_connection_status_t _connect_status;
    bool _new_context_set;
    bool _is_context_active;
    RegistrationStatus _reg_status;
    RadioAccessTechnology _current_act;
    mbed::Callback<void()> _urc_funcs[C_MAX];
};

} // namespace mbed

#endif // AT_CELLULAR_NETWORK_H_
