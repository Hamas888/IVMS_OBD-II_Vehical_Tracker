/*
 ====================================================================================================
 * File:        Movement.h
 * Author:      Hamas Saeed
 * Version:     Rev_1.0.0
 * Date:        March 25 2024
 * Brief:       This file provides motion, accelration, and breaking functionality
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

#ifndef MOVEMENT_H
#define MOVEMENT_H

/* Includes */
#include <Wire.h>
#include <MPU6050.h>

/* Extern Variable */
extern MPU6050   mpu;

extern float    accelOffsetX;
extern float    accelOffsetY;
extern float    accelOffsetZ;
extern float    gyroOffsetX;
extern float    gyroOffsetY;
extern float    gyroOffsetZ;

extern int16_t  accelRawX;
extern int16_t  accelRawY;
extern int16_t  accelRawZ;
extern int16_t  gyroRawX;
extern int16_t  gyroRawY;
extern int16_t  gyroRawZ;

/* Function Prototypes */
void mpuInt();
void calibrateMPU6050();
void detectLaneChange(float gyroY, float gyroZ);
void detectEvents(float accX, float accY, float accZ, float gyroX, float gyroY, float gyroZ); 

#endif // MOVEMENT_H