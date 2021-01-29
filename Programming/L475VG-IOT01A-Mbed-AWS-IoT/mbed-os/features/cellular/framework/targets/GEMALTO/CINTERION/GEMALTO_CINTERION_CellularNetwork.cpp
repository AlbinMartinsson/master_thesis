/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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

#include "GEMALTO_CINTERION_CellularNetwork.h"
#include "GEMALTO_CINTERION_CellularStack.h"
#include "GEMALTO_CINTERION_Module.h"

using namespace mbed;

GEMALTO_CINTERION_CellularNetwork::GEMALTO_CINTERION_CellularNetwork(ATHandler &atHandler) : AT_CellularNetwork(atHandler)
{
}

GEMALTO_CINTERION_CellularNetwork::~GEMALTO_CINTERION_CellularNetwork()
{
}

#if !NSAPI_PPP_AVAILABLE
NetworkStack *GEMALTO_CINTERION_CellularNetwork::get_stack()
{
    if (!_stack) {
        _stack = new GEMALTO_CINTERION_CellularStack(_at, _apn, _cid, _ip_stack_type);
    }
    return _stack;
}
#endif // NSAPI_PPP_AVAILABLE

bool GEMALTO_CINTERION_CellularNetwork::get_modem_stack_type(nsapi_ip_stack_t requested_stack)
{
#if NSAPI_PPP_AVAILABLE
    return (requested_stack == IPV4_STACK || requested_stack == IPV6_STACK);
#else
    if (GEMALTO_CINTERION_Module::get_model() == GEMALTO_CINTERION_Module::ModelBGS2) {
        return (requested_stack == IPV4_STACK);
    }
    return (requested_stack == IPV4_STACK || requested_stack == IPV6_STACK);
#endif
}

AT_CellularNetwork::RegistrationMode GEMALTO_CINTERION_CellularNetwork::has_registration(RegistrationType reg_type)
{
    if (GEMALTO_CINTERION_Module::get_model() == GEMALTO_CINTERION_Module::ModelEMS31) {
        return (reg_type == C_EREG) ? RegistrationModeLAC : RegistrationModeDisable;
    }
    if (GEMALTO_CINTERION_Module::get_model() == GEMALTO_CINTERION_Module::ModelBGS2) {
        if (reg_type == C_GREG) {
            return RegistrationModeEnable;
        }
        return (reg_type == C_REG) ? RegistrationModeLAC : RegistrationModeDisable;
    }
    return (reg_type == C_REG || reg_type == C_GREG || reg_type == C_EREG) ? RegistrationModeLAC : RegistrationModeDisable;
}

nsapi_error_t GEMALTO_CINTERION_CellularNetwork::set_access_technology_impl(RadioAccessTechnology opsAct)
{
    _op_act = RAT_UNKNOWN;
    return NSAPI_ERROR_UNSUPPORTED;
}
