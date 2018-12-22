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
#include "ds1307.h"
#include "../i2c/i2c_master.h"
#include <stdint.h>

//! convert bcd value to decimal
static uint8_t ds1307_bcd2dec(uint8_t val) {
	return val - 6 * (val >> 4);
}

//! convert decimal value to bcd
uint8_t ds1307_dec2bcd(uint8_t val) {
	return val + 6 * (val / 10);
}


void ds1307_init()
{
    i2c_init();
}

void ds1307_enable_sq(uint8_t setting)
{
    i2c_start_wait(DS1307_ADDR | I2C_WRITE);
    i2c_write(0x07);//set register pointer to sq register
    i2c_write(setting);
    i2c_stop();
    
}

void ds1307_get_datetime(rtc_datetime_t *dtp)
{
  i2c_start_wait(DS1307_ADDR | I2C_WRITE);
  i2c_write(0x00);//set register pointer to 0
  i2c_stop(); // stop oscillator
  i2c_rep_start(DS1307_ADDR | I2C_READ);

  dtp->time.second = ds1307_bcd2dec(i2c_readAck() & 0x7F);
  dtp->time.minute = ds1307_bcd2dec(i2c_readAck());
  dtp->time.hour = ds1307_bcd2dec(i2c_readAck());
  i2c_readAck();
  dtp->date.dom = ds1307_bcd2dec(i2c_readAck());
  dtp->date.month = ds1307_bcd2dec(i2c_readAck());
  dtp->date.year = ds1307_bcd2dec(i2c_readNak());
  i2c_stop();  
}


void ds1307_set_datetime(rtc_datetime_t *dtp){
    //write date
    i2c_start_wait(DS1307_ADDR | I2C_WRITE);
    i2c_write(0x00);//set register pointer to 0
    i2c_write(ds1307_dec2bcd(dtp->time.second));
    i2c_write(ds1307_dec2bcd(dtp->time.minute));
    i2c_write(ds1307_dec2bcd(dtp->time.hour));
    i2c_write(ds1307_dec2bcd(dtp->date.dow));
    i2c_write(ds1307_dec2bcd(dtp->date.dom));
    i2c_write(ds1307_dec2bcd(dtp->date.month));
    i2c_write(ds1307_dec2bcd(dtp->date.year));
//    i2c_write(0b00010000); // write to register 7: Square wave settings
    i2c_stop();
}
