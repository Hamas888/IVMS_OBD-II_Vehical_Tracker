 /*
 ====================================================================================================
 * File:        Debug.h
 * Author:      Hamas Saeed
 * Version:     Rev_1.0.0
 * Date:        March 25 2024
 * Brief:       This file provides debug functionality
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
 
#ifndef DEBUG_H
#define DEBUG_H

#define INCLUDE_DEBUG
#define SERIAL_DEBUG
#define GSM_SERIAL_DEBUG
#define STN_SERIAL_DEBUG
#define GYRO_SERIAL_DEBUG

#ifdef INCLUDE_DEBUG

#ifdef SERIAL_DEBUG
  #define DEBUG(x)                  Serial.print(x)
  #define DEBUGLN(x)                Serial.println(x)
  #define DEBUGF(x, y)              Serial.printf(x, y)
#else
  #define DEBUG(x)
  #define DEBUGLN(x)
  #define DEBUGF(x, y)
#endif // SERIAL_DEBUG

#ifdef GSM_SERIAL_DEBUG
  #define GSM_DEBUG(x)              Serial.print(x)
  #define GSM_DEBUGLN(x)            Serial.println(x)
  #define GSM_DEBUGF(x, y)          Serial.printf(x, y)
  #define GSM_DEBUGFF(x, y, z)      Serial.printf(x, y, z)
  #define GSM_DEBUGFFF(x, y, z, v)  Serial.printf(x, y, z,v)
#else
  #define GSM_DEBUG(x)
  #define GSM_DEBUGLN(x)
  #define GSM_DEBUGF(x, y)
  #define GSM_DEBUGFF(x, y, z)
  #define GSM_DEBUGFFF(x, y, z, v)
#endif // GSM_SERIAL_DEBUG

#ifdef STN_SERIAL_DEBUG
  #define STN_DEBUG(x)              Serial.print(x)
  #define STN_DEBUGLN(x)            Serial.println(x)
  #define STN_DEBUGF(x, y)          Serial.printf(x, y)
#else
  #define STN_DEBUG(x)
  #define STN_DEBUGLN(x)
  #define STN_DEBUGF(x, y)
#endif // STN_SERIAL_DEBUG

#ifdef GYRO_SERIAL_DEBUG
  #define GYRO_DEBUG(x)             Serial.print(x)
  #define GYRO_DEBUGLN(x)           Serial.println(x)
  #define GYRO_DEBUGF(x, y)         Serial.printf(x, y)
#else
  #define GYRO_DEBUG(x)
  #define GYRO_DEBUGLN(x)
  #define GYRO_DEBUGF(x, y)
#endif // GYRO_SERIAL_DEBUG

#endif // INCLUDE_DEBUG

#endif // DEBUG_H