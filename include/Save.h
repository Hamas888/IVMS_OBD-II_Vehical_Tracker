/*
 ====================================================================================================
 * File:        Save.h
 * Author:      Hamas Saeed
 * Version:     Rev_1.0.0
 * Date:        March 25 2024
 * Brief:       This file provides flash read and write functionality
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

#ifndef SAVE_H
#define SAVE_H

/* Includes */
#include <stdint.h>

/* Structures & Enums */
typedef struct {
  char tagMac     [20];
  char devMac     [20];
  char apn        [30];
  char user       [10];
  char pass       [10];
  char defSett    [10];
  char firmVer    [10];

  uint32_t stnBaudrate;

  bool stnBaudRateSetFlag;
  bool updateFlag;
} Settings;

/* Function Prototypes */
void settingInt(Settings& settings);
void readSettings(int address, Settings& settings);
void saveSettings(int address, const Settings& settings);

#endif // SAVE_H