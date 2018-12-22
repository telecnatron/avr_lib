#ifndef DS1820_H
#define DS1820_H
// -----------------------------------------------------------------------------
// Copyright 2018 Stephen Stebbing. telecnatron.com
//
//    Licensed under the Telecnatron License, Version 1.0 (the “License”);
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//        https://telecnatron.com/software/licenses/
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an “AS IS” BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.
// -----------------------------------------------------------------------------   
/**
 * @file   ds1820.h
 * @author Stephen Stebbing 
 * @date   Mon Dec  9 16:40:33 2013
 * 
 * @brief  Interface for ds1820 one-wire temperature sensor.
 *    * Assume that a single ds1820 is the only device on the bus.
 *    * External pull-up resistor is used.
 *    * Temperature is read in the default 12-bit mode.
 */
#include "../onewire.h"

// Convert temp as read from sensor (2's compiment signed) to float
#define DS1820_RAW_TO_FLOAT(t) t/16.0;

// function declarations
//! Detect a presence on the bus, return 0 (DS1820_ERROR_NONE) if presence detected, none_zero otherwise
//! Sets ds1820_error variabl
uint8_t ds1820_init();

//! Return float representation of a temperature as read by ds1820
//! ie a two's compilment, sign extended number being the temp in 16ths of a degree C
float ds1820_to_float(int16_t temp);

//! Signal the device to start convertion
uint8_t ds1820_start_conversion();

//! Start conversion, wait for completion, place result in *temp
//! return 0 on success, non-zero on error
//! XXX not sure if this actually works.
uint8_t ds1820_convert_and_read_temperature(float *temp);

//! Read temperature info *temp, ds1820_start_conversion() should have been called and coversion completed (~1ms) prior to calling.
//! return 0 on success, non-zero on error
//! XXX not sure if this actually works.
uint8_t ds1820_read_temperature(float *temp);

//! Read the scratchpad and calculate the crc.
// Return true if the calculated crc matches the crc stored in the scratchpad, false otherwise.
// XXX note: this function has not been tested.
uint8_t ds1820_verify_scratchpad_crc();
#endif
