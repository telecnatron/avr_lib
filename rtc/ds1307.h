#ifndef DS1307_H
#define DS1307_H
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
// -----------------------------------------------------------------------------
// Based on code:
// copyright (c) Davide Gironi, 2013
// Released under GPLv3.
// Please refer to LICENSE file for licensing information.
// -----------------------------------------------------------------------------
#include "../rtc.h"

// parameters for ds1307_enable_sq
#define DS1307_SQ_OFF   0b00000000 
#define DS1307_SQ_1     0b00010000
#define DS1307_SQ_4096  0b00010001
#define DS1307_SQ_8192  0b00010010
#define DS1307_SQ_32768 0b00010011

// i2c address, this cannot be changed
// Note that it is shifted left as required when starting i2c transaction
#define DS1307_ADDR (0x68<<1)

//! Initialise the device 
void ds1307_init();

//! Read the datetime from the device into passed rtc_datetime_t structure address
void ds1307_get_datetime(rtc_datetime_t *dtp);

//! Set the datetime from the values contained in the rtc_datetime_t structure at the passed address
void ds1307_set_datetime(rtc_datetime_t *dtp);


//! Enable or disable the square wave output, to the passed setting which should be
//! one of the DS1307_SQ_xxx defines above
void ds1307_enable_sq(uint8_t setting);






#endif
