// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include "BLECast.h"

#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_err.h"

static esp_ble_adv_data_t _adv_config = {
        .set_scan_rsp        = false,
        .include_name        = true,
        .include_txpower     = false,
        .min_interval        = 0x0500,
        .max_interval        = 0x2000,
        .appearance          = 0,
        .manufacturer_len    = 0,
        .p_manufacturer_data = NULL,
        .service_data_len    = 0,
        .p_service_data      = NULL,
        .service_uuid_len    = 0,
        .p_service_uuid      = NULL,
        .flag                = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT)
};

static esp_ble_adv_params_t _adv_params = {
        .adv_int_min         = 0x0700,
        .adv_int_max         = 0x1000,
        .adv_type            = ADV_TYPE_NONCONN_IND,
        .own_addr_type       = BLE_ADDR_TYPE_PUBLIC,
        .peer_addr           = {0x00, },
        .peer_addr_type      = BLE_ADDR_TYPE_PUBLIC,
        .channel_map         = ADV_CHNL_ALL,
        .adv_filter_policy   = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

bool BLECast::_init_gap(){
    esp_err_t ret;
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

    if (esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT)) {
        return false;
    }
    if (esp_bt_controller_init(&bt_cfg)) {
        return false;
    }
    if (esp_bt_controller_enable(ESP_BT_MODE_BLE)) {
        return false;
    }

    esp_bluedroid_status_t bt_state = esp_bluedroid_get_status();
    if(bt_state == ESP_BLUEDROID_STATUS_UNINITIALIZED){
        if (esp_bluedroid_init()) {
            return false;
        }
    }
    if(bt_state != ESP_BLUEDROID_STATUS_ENABLED){
        if (esp_bluedroid_enable()) {
            return false;
        }
    }

    // MAX POWER!!!
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, ESP_PWR_LVL_P7);

    esp_ble_gap_config_adv_data_raw((uint8_t*)m_payload.data(), m_payload.length());
    esp_ble_gap_start_advertising(&_adv_params);

    running = true;

    return true;
}

bool BLECast::_stop_gap()
{
    esp_bluedroid_disable();
    esp_bluedroid_deinit();

    running = false;

    return true;
}

void BLECast::setAdvData(std::string data) {
    if (data.length() > ESP_BLE_ADV_DATA_LEN_MAX) {
        return;
    }
    m_payload = data;
}


BLECast::BLECast(std::string localName)
{
    local_name = localName;

    char cdata[2];
    cdata[0] = localName.length() + 1;
    cdata[1] = ESP_BLE_AD_TYPE_NAME_CMPL;  // 0x09
    setAdvData(std::string(cdata, 2) + localName);
}

BLECast::~BLECast(void)
{
    _stop_gap();
}

bool BLECast::begin()
{
    return _init_gap();
}

void BLECast::extendDeviceName(std::string payload) {
    char nameHeader[2];
    nameHeader[0] = local_name.length() + 1 + payload.length();
    nameHeader[1] = ESP_BLE_AD_TYPE_NAME_CMPL;  // 0x09

    setAdvData(std::string(nameHeader, 2) + local_name + payload);

    if (running) {
        esp_ble_gap_config_adv_data_raw((uint8_t*)m_payload.data(), m_payload.length());
    }
}

void BLECast::end()
{
    _stop_gap();
}
