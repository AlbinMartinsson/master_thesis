/*
 * Copyright (c) 2016-2018, Arm Limited and affiliates.
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

#ifndef WS_COMMON_H_
#define WS_COMMON_H_


#include "ns_types.h"
#include "fhss_api.h"
#include "fhss_config.h"
#include "net_fhss.h"
#include "6LoWPAN/ws/ws_common_defines.h"
#include "6LoWPAN/ws/ws_neighbor_class.h"

struct ws_pan_information_s;
struct ws_neighbor_class_s;

typedef struct parent_info_s {
        uint16_t pan_id;            /**< PAN ID */
        uint8_t addr[8];            /**< address */
        uint8_t link_quality;       /**< LQI value measured during reception of the MPDU */
        int8_t signal_dbm;          /**< This extension for normal IEEE 802.15.4 Data indication */
        ws_pan_information_t pan_information;
        ws_utt_ie_t ws_utt;
        ws_us_ie_t ws_us;
        uint32_t timestamp;            /**< Timestamp when packet was received */
}parent_info_t;

typedef struct ws_info_s {
    char network_name[33]; // Network name max 32 octets + terminating 0.
    uint16_t network_pan_id;

    trickle_t trickle_pan_config_solicit;
    trickle_t trickle_pan_config;
    trickle_t trickle_pan_advertisement_solicit;
    trickle_t trickle_pan_advertisement;
    uint8_t rpl_state; // state from rpl_event_t
    uint8_t pas_requests; // Amount of PAN solicits sent
    parent_info_t parent_info;
    uint32_t pan_version_timer;            /**< border router version udate timeout */
    uint32_t pan_version_timeout_timer;    /**< routers will fallback to previous state after this */
    uint8_t gtkhash[32];
    bool configuration_learned:1;

    struct ws_pan_information_s pan_information;
    ws_hopping_schedule_t hopping_schdule;
    struct ws_neighbor_class_s neighbor_storage;
    struct fhss_timer *fhss_timer_ptr; // Platform adaptation for FHSS timers.
    struct fhss_api *fhss_api;
} ws_info_t;

#ifdef HAVE_WS

int8_t ws_common_regulatory_domain_config(protocol_interface_info_entry_t *cur);

int8_t ws_common_allocate_and_init(protocol_interface_info_entry_t *cur);

void ws_common_seconds_timer(protocol_interface_info_entry_t *cur, uint32_t seconds);

void ws_common_fast_timer(protocol_interface_info_entry_t *cur, uint16_t ticks);

void ws_common_neighbor_update(protocol_interface_info_entry_t *cur, const uint8_t *ll_address);

#define ws_info(cur) ((cur)->ws_info)
#else
#define ws_info(cur) ((ws_info_t *) NULL)
#define ws_common_seconds_timer(cur, seconds)
#define ws_common_neighbor_update(cur, ll_address) ((void) 0)
#define ws_common_fast_timer(cur, ticks) ((void) 0)


#endif //HAVE_WS
#endif //WS_COMMON_H_
