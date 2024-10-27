/*
 ====================================================================================================
 * File:        OTA.h
 * Author:      Hamas Saeed
 * Version:     Rev_1.0.1
 * Date:        August 2 2024
 * Brief:       This file provides over the air firmware update functionality
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

#ifndef OTA_H
#define OTA_H

/* Includes */
#include <Update.h>

/* Extern Variable */
extern bool     updateAvailable;
extern bool     otaStarting;
extern uint32_t knownFileSize; 

/* Function Prototypes */
void checkUpdate(Settings &settings);
void perfomeOta(Settings &settings, int version);
bool checkOtaVer(Settings &settings, int version); 
String convertVersionString(const String &version);

#endif // OTA_H