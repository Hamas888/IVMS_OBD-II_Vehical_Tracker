/*
 ====================================================================================================
 * File:        Scan.h
 * Author:      Hamas Saeed
 * Version:     Rev_1.0.0
 * Date:        March 25 2024
 * Brief:       This file provides BLE tag scaning & detection functionality
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

#ifndef SCAN_H
#define SCAN_H

/* Includes */
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

/* Macros */
#define TAG_MAC             "c3:00:00:12:37:6d"
#define TAG_PROXIMITY       -99

/* Extern Volatile Variable */
extern volatile bool        tagDetected;

/* Classes */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  public:
    void onResult(BLEAdvertisedDevice advertisedDevice);
};

/* Function Prototypes */
void scanInt();
void searchTag();

#endif // SCAN_H