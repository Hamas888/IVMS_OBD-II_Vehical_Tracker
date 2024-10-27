/*
 ====================================================================================================
 * File:        BLE.h
 * Author:      Hamas Saeed
 * Version:     Rev_1.0.0
 * Date:        March 25 2024
 * Brief:       This file provides BLE functionality
 * 
 ====================================================================================================
 * License: 
 * This file is licensed under the GNU Affero General Public License (AGPL) v3.0.
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 * https://www.gnu.org/licenses/agpl-3.0.en.html
 * 
 * Commercial licensing: For commercial use of this software, please contact Hamas Saeed at 
 * hamasaeed@gmail.com.
 * 
 * Distributed under the AGPLv3 License. Software is provided "AS IS," without any warranties 
 * or conditions of any kind, either express or implied.
 *
 =====================================================================================================
 */

#ifndef BLE_H
#define BLE_H

/* Includes */
#include "Config.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <wifi.h>

/* Macros */
#define BLE_PASSWORD              "1155"

/* Extern Variable */
extern BLEServer    *pServer;
extern bool         deviceConnected;
extern bool         oldDeviceConnected;
extern bool         convert;
extern String       rxString;
extern std::string  rxValue; 

/* Classes */
class MyServerCallbacks : public BLEServerCallbacks {
  public:
    void onConnect(BLEServer* pServer);
    void onDisconnect(BLEServer* pServer);
};

class MyCallbacks : public BLECharacteristicCallbacks {
  public:
    void onWrite(BLECharacteristic* pCharacteristic);
};

/* Function Prototypes */
void bleInit();
void bleSendWrite();
void geather_Data();

#endif // BLE_H