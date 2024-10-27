/*
 ====================================================================================================
 * File:        STN.h
 * Author:      Hamas Saeed
 * Version:     Rev_1.0.0
 * Date:        March 25 2024
 * Brief:       This file provides STN functionality to connect with OBD II port of vehical
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

#ifndef STN_H
#define STN_H

/* Includes */
#include "Config.h"

/* Macros */
#define STN    Serial2

/* Extern Variable */
extern bool stnSleepFlag;
extern bool sleepFlag;

/* Structures & Enums */
typedef enum {
  CAR_MONITOR_STATUS_SINCE_DTC_CLEARED      = 0,                       // PID 01 - Monitor status since DTCs cleared
  CAR_FREEZE_DTC                            = 1,                       // PID 02 - Freeze DTC
  CAR_FUEL_SYSTEM_STATUS                    = 2,                       // PID 03 - Fuel system status
  CAR_ENGINE_LOAD                           = 3,                       // PID 04 - Calculated engine load
  CAR_COOLANT_TEMP                          = 4,                       // PID 05 - Engine coolant temperature
  CAR_SHORT_TERM_FUEL_TRIM_BANK1            = 5,                       // PID 06 - Short-term fuel trim—Bank 1
  CAR_LONG_TERM_FUEL_TRIM_BANK1             = 6,                       // PID 07 - Long-term fuel trim—Bank 1
  CAR_SHORT_TERM_FUEL_TRIM_BANK2            = 7,                       // PID 08 - Short-term fuel trim—Bank 2
  CAR_LONG_TERM_FUEL_TRIM_BANK2             = 8,                       // PID 09 - Long-term fuel trim—Bank 2
  CAR_FUEL_PRESSURE                         = 9,                       // PID 0A - Fuel pressure
  CAR_INTAKE_MANIFOLD_PRESSURE              = 10,                      // PID 0B - Intake manifold absolute pressure
  CAR_ENGINE_RPM                            = 11,                      // PID 0C - Engine RPM
  CAR_VEHICLE_SPEED                         = 12,                      // PID 0D - Vehicle speed
  CAR_TIMING_ADVANCE                        = 13,                      // PID 0E - Timing advance
  CAR_INTAKE_AIR_TEMP                       = 14,                      // PID 0F - Intake air temperature
  CAR_MAF_AIR_FLOW_RATE                     = 15,                      // PID 10 - MAF air flow rate
  CAR_THROTTLE_POSITION                     = 16,                      // PID 11 - Throttle position
  CAR_SECONDARY_AIR_STATUS                  = 17,                      // PID 12 - Secondary air status
  CAR_OXYGEN_SENSORS_PRESENT_2_BANKS        = 18,                      // PID 13 - Oxygen sensors present (in 2 banks)
  CAR_OXYGEN_SENSOR_VOLTAGE_BANK1_SENSOR1   = 19,                      // PID 14 - Bank 1, Sensor 1: Oxygen sensor voltage, Short-term fuel trim
  CAR_OXYGEN_SENSOR_VOLTAGE_BANK1_SENSOR2   = 20,                      // PID 15 - Bank 1, Sensor 2: Oxygen sensor voltage, Short-term fuel trim
  CAR_OXYGEN_SENSOR_VOLTAGE_BANK1_SENSOR3   = 21,                      // PID 16 - Bank 1, Sensor 3: Oxygen sensor voltage, Short-term fuel trim
  CAR_OXYGEN_SENSOR_VOLTAGE_BANK1_SENSOR4   = 22,                      // PID 17 - Bank 1, Sensor 4: Oxygen sensor voltage, Short-term fuel trim
  CAR_OXYGEN_SENSOR_VOLTAGE_BANK2_SENSOR1   = 23,                      // PID 18 - Bank 2, Sensor 1: Oxygen sensor voltage, Short-term fuel trim
  CAR_OXYGEN_SENSOR_VOLTAGE_BANK2_SENSOR2   = 24,                      // PID 19 - Bank 2, Sensor 2: Oxygen sensor voltage, Short-term fuel trim
  CAR_OXYGEN_SENSOR_VOLTAGE_BANK2_SENSOR3   = 25,                      // PID 1A - Bank 2, Sensor 3: Oxygen sensor voltage, Short-term fuel trim
  CAR_OXYGEN_SENSOR_VOLTAGE_BANK2_SENSOR4   = 26,                      // PID 1B - Bank 2, Sensor 4: Oxygen sensor voltage, Short-term fuel trim
  CAR_OBD_STANDARDS                         = 27,                      // PID 1C - OBD standards this vehicle conforms to
  CAR_OXYGEN_SENSORS_PRESENT_4_BANKS        = 28,                      // PID 1D - Oxygen sensors present (in 4 banks)
  CAR_AUX_INPUT_STATUS                      = 29,                      // PID 1E - Auxiliary input status
  CAR_RUNTIME_SINCE_ENGINE_START            = 30,                      // PID 1F - Run time since engine start
  CAR_SUPPORTED_PIDS_21_40                  = 31,                      // PID 20 - PIDs supported [21-40]
  CAR_DISTANCE_TRAVELED_WITH_MIL_ON         = 32,                      // PID 21 - Distance traveled with MIL on
  CAR_FUEL_RAIL_PRESSURE                    = 33,                      // PID 22 - Fuel rail pressure (relative to manifold vacuum)
  CAR_FUEL_RAIL_GAUGE_PRESSURE              = 34,                      // PID 23 - Fuel rail gauge pressure (diesel)
  CAR_OXYGEN_SENSOR1_LAMBDA_VOLTAGE         = 35,                      // PID 24 - O2 Sensor 1 - Equivalence ratio (λ) & voltage
  CAR_OXYGEN_SENSOR2_LAMBDA_VOLTAGE         = 36,                      // PID 25 - O2 Sensor 2 - Equivalence ratio (λ) & voltage
  CAR_OXYGEN_SENSOR3_LAMBDA_VOLTAGE         = 37,                      // PID 26 - O2 Sensor 3 - Equivalence ratio (λ) & voltage
  CAR_OXYGEN_SENSOR4_LAMBDA_VOLTAGE         = 38,                      // PID 27 - O2 Sensor 4 - Equivalence ratio (λ) & voltage
  CAR_OXYGEN_SENSOR5_LAMBDA_VOLTAGE         = 39,                      // PID 28 - O2 Sensor 5 - Equivalence ratio (λ) & voltage
  CAR_OXYGEN_SENSOR6_LAMBDA_VOLTAGE         = 40,                      // PID 29 - O2 Sensor 6 - Equivalence ratio (λ) & voltage
  CAR_OXYGEN_SENSOR7_LAMBDA_VOLTAGE         = 41,                      // PID 2A - O2 Sensor 7 - Equivalence ratio (λ) & voltage
  CAR_OXYGEN_SENSOR8_LAMBDA_VOLTAGE         = 42,                      // PID 2B - O2 Sensor 8 - Equivalence ratio (λ) & voltage
  CAR_COMMANDED_EGR                         = 43,                      // PID 2C - Commanded EGR
  CAR_EGR_ERROR                             = 44,                      // PID 2D - EGR Error
  CAR_COMMANDED_EVAP_PURGE                  = 45,                      // PID 2E - Commanded evaporative purge
  CAR_FUEL_TANK_LEVEL                       = 46,                      // PID 2F - Fuel level input
  CAR_WARM_UPS_SINCE_CODES_CLEARED          = 47,                      // PID 30 - Number of warm-ups since codes cleared
  CAR_DISTANCE_TRAVELED_SINCE_CODES_CLEARED = 48,                      // PID 31 - Distance traveled since codes cleared
  CAR_EVAP_SYSTEM_VAPOR_PRESSURE            = 49,                      // PID 32 - Evaporative system vapor pressure
  CAR_ABSOLUTE_BAROMETRIC_PRESSURE          = 50,                      // PID 33 - Absolute barometric pressure
  CAR_OXYGEN_SENSOR1_LAMBDA_CURRENT         = 51,                      // PID 34 - O2 Sensor 1 - Equivalence ratio (λ) & current
  CAR_OXYGEN_SENSOR2_LAMBDA_CURRENT         = 52,                      // PID 35 - O2 Sensor 2 - Equivalence ratio (λ) & current
  CAR_OXYGEN_SENSOR3_LAMBDA_CURRENT         = 53,                      // PID 36 - O2 Sensor 3 - Equivalence ratio (λ) & current
  CAR_OXYGEN_SENSOR4_LAMBDA_CURRENT         = 54,                      // PID 37 - O2 Sensor 4 - Equivalence ratio (λ) & current
  CAR_OXYGEN_SENSOR5_LAMBDA_CURRENT         = 55,                      // PID 38 - O2 Sensor 5 - Equivalence ratio (λ) & current
  CAR_OXYGEN_SENSOR6_LAMBDA_CURRENT         = 56,                      // PID 39 - O2 Sensor 6 - Equivalence ratio (λ) & current
  CAR_OXYGEN_SENSOR7_LAMBDA_CURRENT         = 57,                      // PID 3A - O2 Sensor 7 - Equivalence ratio (λ) & current
  CAR_OXYGEN_SENSOR8_LAMBDA_CURRENT         = 58,                      // PID 3B - O2 Sensor 8 - Equivalence ratio (λ) & current
  CAR_CATALYST_TEMP_BANK1_SENSOR1           = 59,                      // PID 3C - Catalyst temperature: Bank 1, Sensor 1
  CAR_CATALYST_TEMP_BANK2_SENSOR1           = 60,                      // PID 3D - Catalyst temperature: Bank 2, Sensor 1
  CAR_FUEL_INJECTION_TIMING                 = 61,                      // PID 3E - Fuel injection timing
  CAR_ENGINE_FUEL_RATE                      = 62,                      // PID 3F - Engine fuel rate
  CAR_ENGINE_TORQUE_PERCENT                 = 63,                      // PID 40 - Engine torque percent
  CAR_ENGINE_REF_TORQUE                     = 64,                      // PID 41 - Engine reference torque
  CAR_ENGINE_ACTUAL_TORQUE                  = 65,                      // PID 42 - Engine actual torque  
} CARPIDS;

bool stnInt();
void stnSleep();
void fetchDtc();
void stnWakeSetting();
bool getherParameters();
String queryPID(CARPIDS pid);
void parseSupportedPIDs(const String& response);
void parseDTCString(String &str, uint8_t &totalDTC, uint16_t dtc[]);
template <typename T>
bool computeParameter(String& response, uint8_t index, T& parameter, uint16_t wildCard);

#endif // STN_H