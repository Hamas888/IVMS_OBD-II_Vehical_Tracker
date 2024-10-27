 /*
 ====================================================================================================
 * File:        Config.h
 * Author:      Hamas Saeed
 * Version:     Rev_1.0.0
 * Date:        March 25 2024
 * Brief:       This file provides Device configurations
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

#ifndef CONFIG_H
#define CONFIG_H

/* Includes */
#include <Arduino.h>
#include <HardwareSerial.h>
#include "EEPROM.h"
#include "esp_task_wdt.h"
#include "Debug.h"
#include "Save.h"
#include "STN.h"
#include "ESP_RELAYS.h"

/* Macros */
#define SETTINGS_ADDRESS    0
#define SETTINGS_SIZE       250
#define FIRMWARE_VERSION    "1.0.1"

#define STN_WAKE_THRESHOLD  890

#define RELAY_PIN_1         2                              // sw1
#define RELAY_PIN_2         4                              // sw2
#define RELAY_PIN_3         12                             // sw1
#define RELAY_PIN_4         13                             // sw2
#define RELAY_PIN_5         15                             // sw1
#define KEY_PIN             5                              // Digital read key insert 

#define MAIN_BATTERY_0      14                             // Analog read main battery 0
#define MAIN_BATTERY_1      27                             // Analog read main battery 1
#define BACKUP_BATTERY      32                             // Analog read backup battery

/* Extern Volatile Variable */
extern volatile bool        relayHFlag;
extern volatile bool        relayLFlag;

extern volatile bool        stnInitialized;
extern volatile bool        stnNotAvailable;
extern volatile bool        gsmInitialized;
extern volatile bool        mpuInitialized;
extern volatile bool        checkOtaFlag;
extern volatile bool        dataPosted;
extern volatile bool        harshAccelGSM;
extern volatile bool        harshBreakGSM;
extern volatile bool        harshBreakBLE;
extern volatile bool        harshAccelBLE;

/* Extern Variable */
extern bool                 movment;
extern bool                 unpluged;
extern bool                 ignition;
extern bool                 ignitionStatusFlag;
extern bool                 keyCheck;
extern bool                 engineOFF;
extern bool                 dtcCheck;
extern uint8_t              vehicleCoolantTemp;
extern uint8_t              vehicleSpeed;
extern uint16_t             fuelStatus;
extern uint16_t             vehicalFuel;
extern uint16_t             vehicleRPM;
extern uint32_t             vehicleOdoMeter;
extern float                engineload;
extern float                vehicleBattery;
extern float                stnVoltage;
extern float                magnitude;
extern float                accelX;
extern float                accelY;
extern float                accelZ;
extern int16_t              temperature;

extern float                GPSaltitude;
extern float                GPSspeed;
extern float                GPSaccuracy;
extern float                GPSlatitude;
extern float                GPSlongitude;
extern int                  GPSyear;
extern int                  GPSmonth;
extern int                  GPSdate;
extern int                  GPShours;
extern int                  GPSminutes;
extern int                  GPSseconds;
extern int                  GPSs;
extern int                  GLONASSs;
extern int                  BEIDOUs;
extern int                  vsat;
extern int                  usat; 

extern String               dtc_03_String;
extern String               dtc_07_String;

extern uint16_t             wakeBattery_0;
extern uint16_t             wakeBattery_1;

extern Settings             devSettings;

extern TaskHandle_t         GsmTask;
extern TaskHandle_t         GyroTask;
extern TaskHandle_t         StnTask;

/* Function Prototypes */
void valueCheck();
void circuitsON();
void circuitsOFF();
void portalButtonCheck();
void gsmTask(void *pvParameters); 
void gyroTask(void *pvParameters);
void stnTask(void *pvParameters);


#endif // CONFIG_H