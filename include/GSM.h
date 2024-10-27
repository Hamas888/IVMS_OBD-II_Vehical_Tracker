/*
 ====================================================================================================
 * File:        GSM.h
 * Author:      Hamas Saeed
 * Version:     Rev_1.0.0
 * Date:        March 25 2024
 * Brief:       This file provides GSM & GPS functionality sim7600
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

#ifndef GSM_H
#define GSM_H

/* To Import Specific Module */
#define TINY_GSM_MODEM_SIM7600                              // Only importing sim7600 from TinyGsmClient

/* Includes */
#include <TinyGsmClient.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "Save.h"

/* Macros */
#define GSM_SERIAL          Serial1

#define MODEM_TX            25                              // sim7600_RX->(25)esp32_Tx2  -Serial2
#define MODEM_RX            26                              // sim7600_TX->(26)esp32_Rx2  -Serial2


/* Extern Variables */
extern TinyGsm              modem;
extern TinyGsmClient        client;

extern HardwareSerial       modemHS;

extern char                 fw_resource[];
extern char                 ver_resource[];
extern char                 imei[];
extern char                 sim_iccid[];
extern const char           downRes[];
extern const char           endRes[];
extern const char           verFile[];
extern const char           otaFile[];
extern const char           server[];
extern const int            port;

extern String               rxImei;
extern String               rxName;
extern String               rxRef;
extern String               rxCom;
extern String               rxbtnRef;
extern String               rxbtnCom;
extern bool                 rxFlag;

/* Function Prototypes */
bool gpsInt();
bool getGPS();
bool fetchGPS();
uint8_t getRssi();
void serializeData();
bool checkSMS(int index);
bool gsmInt(Settings &settings);
bool postdata(Settings &settings);
void parseResponse(const String &line);
String extractValue(const String &line, const String &startKey, const String &endKey);
String sendATcommand(const char* command, const char* expected_response, unsigned int timeout);

#endif // GSM_H
