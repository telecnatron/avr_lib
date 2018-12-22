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
#include "ds1302.h"

#include <util/delay.h>

// define DS1302_LOGGING to have log messages displayed
#ifdef DS1302_LOGGING
#include "lib/log.h"
#define DS1302_LOG(fmt, msg...) LOG_INFO_FP(fmt, msg)
#else
#define DS1302_LOG(fmt, msg...) 
#endif

// set clock line 
#define DS1302_CLK_LO() DS1302_PORT &=~ _BV(DS1302_PIN_CLK)
#define DS1302_CLK_HI()	DS1302_PORT |= _BV(DS1302_PIN_CLK)
// set CE line
#define DS1302_CE_LO()  DS1302_PORT &=~ _BV(DS1302_PIN_CE)
#define DS1302_CE_HI()  DS1302_PORT |= _BV(DS1302_PIN_CE)
// set DATA line
#define DS1302_DATA_LO()  DS1302_PORT &=~ _BV(DS1302_PIN_DATA)
#define DS1302_DATA_HI()  DS1302_PORT |= _BV(DS1302_PIN_DATA)
// set DATA line as input or output - note pullup is disabled for input
#define DS1302_DATA_IN()  DS1302_DDR &=~ _BV(DS1302_PIN_DATA);DS1302_DATA_LO()
#define DS1302_DATA_OUT() DS1302_DDR |= _BV(DS1302_PIN_DATA)
// read the data line
#define DS1302_DATA_READ() (DS1302_PORTIN & _BV(DS1302_PIN_DATA))
// delay 
#define DS1302_DELAY()    _delay_us(DS1302_DELAY_US)

// -----------------------------------------------------------------------------
// static functions

// ready for transaction
static void ds1302_reset()
{
    // CE and CLK low
    DS1302_CLK_LO();
    DS1302_CE_LO();
    DS1302_DELAY();
    // now put CE high to begin read/write
    DS1302_CE_HI();
}

// CLK is assumed to be high from previous write_byte()
static uint8_t ds1302_read_byte()
{
    uint8_t byte=0;

    // set DATA as input
    DS1302_DATA_IN();
    for(uint8_t i=0; i!=8; ++i){
	// CLK falling edge causes device to put bit on DATA
	DS1302_CLK_LO();
	DS1302_DELAY();
	if(DS1302_DATA_READ()!=0){
	    byte |= 1<<i;
	}
	// CLK high ready for next read
	DS1302_CLK_HI();
	DS1302_DELAY();
    }
    return byte;
}

// write the passed byte to device
static void ds1302_write_byte(uint8_t byte)
{
    // set DATA as output
    DS1302_DATA_OUT();
    DS1302_DELAY(); // give some time from previous transaction
    for(uint8_t i=0; i!=8 ; ++i){
	// clock out the bits, lsb first
	// set DATA to bit value
	DS1302_DATA_LO();
	if(byte & 0x01)
	    DS1302_DATA_HI();
	// stobe CLK
	DS1302_CLK_LO();
	DS1302_DELAY();
	DS1302_CLK_HI();
	byte >>=1; // shift ready for next bit
	DS1302_DELAY();
    }
    // end with CLK hi
}
// -----------------------------------------------------------------------------
// api functions

void ds1302_get_datetime(rtc_datetime_t *dtp)
{
    ds1302_get_datetime_raw(dtp);
    ds1302_datetime_raw2int(dtp);
}

void ds1302_set_datetime(rtc_datetime_t *dtp)
{
    // convert to raw form
    ds1302_datetime_int2raw(dtp);
    // write data
    ds1302_set_datetime_raw(dtp);
    // convert raw back to int
    ds1302_datetime_raw2int(dtp);
}

void ds1302_datetime_int2raw(rtc_datetime_t *dtp)
{
    dtp->time.second = ((dtp->time.second/10)<<4) | (dtp->time.second%10);
    dtp->time.minute = ((dtp->time.minute/10)<<4) | (dtp->time.minute%10);
    if((dtp->time.hour&0x80) == 0)
    {
        dtp->time.hour = ((dtp->time.hour/10)<<4) | (dtp->time.hour%10);
#if 0
    }else {
	// 12 hour is being used - 
	// XXX implement me
#endif
    }
    dtp->date.dow = ((dtp->date.dow/10)<<4) | (dtp->date.dow%10);
    dtp->date.month = ((dtp->date.month/10)<<4) | (dtp->date.month%10);
    dtp->date.year = ((dtp->date.year/10)<<4) | (dtp->date.year%10);
}

void ds1302_datetime_raw2int(rtc_datetime_t *dtp)
{
    dtp->time.second = (((dtp->time.second&0x70)>>4)*10) + (dtp->time.second&0x0f);
    dtp->time.minute = (((dtp->time.minute&0x70)>>4)*10) + (dtp->time.minute&0x0f);
    if((dtp->time.hour&0x80) == 0) {
	// 24 hour is being used
        dtp->time.hour = (((dtp->time.hour&0x30)>>4)*10) + (dtp->time.hour&0x0f);
#if 0
    }  else    {
	// 12 hour is being used - 
	// XXX implement me
#endif
    }
    dtp->date.dom = (((dtp->date.dom&0x30)>>4)*10) + (dtp->date.dom&0x0f);
    dtp->date.month = (((dtp->date.month&0x10)>>4)*10) + (dtp->date.month&0x0f);
    dtp->date.year = (((dtp->date.year&0xf0)>>4)*10) + (dtp->date.year&0x0f);
}

void ds1302_get_datetime_raw(rtc_datetime_t *dtp)
{
    uint8_t *b=(uint8_t *)dtp;

    // burst read -  7 bytes being the data time registers
    ds1302_reset();
    ds1302_write_byte(DS1302_CLK_BURST_READ);
    for(uint8_t i=0; i!=7; ++i){
	*b++=ds1302_read_byte();
    }
    DS1302_CE_LO(); // complete transaction
}

void ds1302_set_datetime_raw(rtc_datetime_t *dtp)
{

    uint8_t *b=(uint8_t *)dtp;
    // burst write 7 bytes
#if 0
    // XXX why doesn't this work?
    ds1302_reset();
    ds1302_write_byte(DS1302_CLK_BURST_WRITE);
    for(uint8_t i=0; i!=7; ++i){
	ds1302_write_byte(*b++);
    }
    DS1302_CE_LO(); // complete transaction
#else
    // XXX we do it the slow way
    uint8_t cmd=DS1302_WRITE_SECONDS;
    for(uint8_t i=0; i!=7; i++){
	ds1302_write(cmd, *b);
	b++;
	cmd+=2;
    }
#endif
}

uint8_t ds1302_tc(uint8_t cmd)
{
    ds1302_reset();
    ds1302_write(DS1302_WRITE_TC, cmd);
    // On success read byte will = the cmd that we just sent.
    return ds1302_read(DS1302_READ_TC);
}


void ds1302_write(uint8_t cmd, uint8_t data)
{
    ds1302_reset();
    ds1302_write_byte(cmd);
    ds1302_write_byte(data);
    DS1302_CE_LO(); // complete transaction
}

uint8_t ds1302_read(uint8_t cmd)
{  
    uint8_t byte;
    ds1302_reset();
    ds1302_write_byte(cmd);
    byte=ds1302_read_byte();
    DS1302_CE_LO(); // complete transaction
    return byte;
}


void ds1302_init()
{
    // CE, DATA and CLK pin as output,
    DS1302_DDR |= (_BV(DS1302_PIN_CE) | _BV(DS1302_PIN_CLK) | _BV(DS1302_PIN_DATA));
    // set them low
    DS1302_PORT &=~ (_BV(DS1302_PIN_CE) | _BV(DS1302_PIN_CLK));

    uint8_t data;
    // disable clk halt mode
    data=ds1302_read(DS1302_READ_SECONDS);
    ds1302_write(DS1302_WRITE_SECONDS, data & 0x7f);

    // set 24 hour mode
    data=ds1302_read(DS1302_READ_HOURS);
    ds1302_write(DS1302_WRITE_HOURS, data & 0x7f);

    // disable write protect
    ds1302_write(DS1302_WRITE_WP,0);
 
}
