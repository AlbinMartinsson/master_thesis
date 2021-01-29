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

#ifndef GEMALTO_CINTERION_CELLULAR_NETWORK_H_
#define GEMALTO_CINTERION_CELLULAR_NETWORK_H_

#include "AT_CellularNetwork.h"

namespace mbed {

class GEMALTO_CINTERION_CellularNetwork : public AT_CellularNetwork {
public:
    GEMALTO_CINTERION_CellularNetwork(ATHandler &atHandler);
    virtual ~GEMALTO_CINTERION_CellularNetwork();

protected:
#if !NSAPI_PPP_AVAILABLE
    virtual NetworkStack *get_stack();
#endif // NSAPI_PPP_AVAILABLE

    virtual RegistrationMode has_registration(RegistrationType reg_type);

    virtual bool get_modem_stack_type(nsapi_ip_stack_t requested_stack);

    virtual nsapi_error_t set_access_technology_impl(RadioAccessTechnology opsAct);
};

} // namespace mbed

#endif // GEMALTO_CINTERION_CELLULAR_NETWORK_H_
