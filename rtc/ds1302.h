#ifndef DS1302_H
#define DS1302_H
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
// ds1302 rtc routines.
//
// Note: internally ds1302 reads and writes year, seconds, etc values as BCD. Here
// BCD values are refered to as 'raw' data. Routines are provided to convert BCD to
// and from integer (uint8_t in fact).
// -----------------------------------------------------------------------------
#include <avr/io.h>
#include "../rtc.h"
// -----------------------------------------------------------------------------
// pin definitions - if there are overriden elsewhere prior, be sure to also define DS1302_DEFS
#ifndef DS1302_DEFS
#warn "Using default definitions"
#define DS1302_DEFS
// all pins must be on the same port
#define DS1302_PORTIN    PIND
#define DS1302_PORT      PORTD
#define DS1302_DDR       DDRD
#define DS1302_PIN_CE    PIN4
#define DS1302_PIN_DATA  PIN5
#define DS1302_PIN_CLK   PIN6
// delay between clock being set high and low, in microseconds
#define DS1302_DELAY_US 2
#endif
// -----------------------------------------------------------------------------
// RTC commands
// these are the write commands
#define DS1302_WRITE_SECONDS 0x80
#define DS1302_WRITE_MINUTES 0x82
#define DS1302_WRITE_HOURS   0x84
#define DS1302_WRITE_DOM     0x86
#define DS1302_WRITE_MONTH   0x88
#define DS1302_WRITE_DOW     0x8a
#define DS1302_WRITE_YEAR    0x8c
#define DS1302_WRITE_WP      0x8e
#define DS1302_WRITE_TC      0x90
// read commands
#define DS1302_READ_SECONDS 0x81
#define DS1302_READ_MINUTES 0x83
#define DS1302_READ_HOURS   0x85
#define DS1302_READ_DOM     0x87
#define DS1302_READ_MONTH   0x89
#define DS1302_READ_DOW     0x8b
#define DS1302_READ_YEAR    0x8d
#define DS1302_READ_WP      0x8f
#define DS1302_READ_TC      0x91
// clk burst commands
#define DS1302_CLK_BURST_READ  0xbf
#define DS1302_CLK_BURST_WRITE 0xbe


// trickle charger:
// See datasheet page 13.
// one diode, 2k resistor
#define DS1302_TC_1D2K    0b10100101
#define DS1302_TC_1D4K    0b10100110
#define DS1302_TC_1D8K    0b10100111
#define DS1302_TC_2D2K    0b10101001
// two diodes, 4k resistor
#define DS1302_TC_2D4K    0b10101010
#define DS1302_TC_2D8K    0b10101011
#define DS1302_TC_DISABLE 0b00001111
// -------------------

/** 
 * Initialise the device. This configures the pins as appropiate.
 * Disables clock halt. Sets to 24 hour mode. Disables write protect.
 */
void ds1302_init();


/** 
 * Convert the raw (bcd) data contained in the passed dtp, to integer
 * XXX Note: uses 24 hour time, 12 hour is not implemented yet
 * @param dtp Pointer to dtp structure containing raw data
 */
void ds1302_datetime_raw2int(rtc_datetime_t *dtp);

/** 
 * Fill out the passed dtp structure with current date and time, raw data as read
 * from the device's registers
 * @param dtp  Pointer to structure of type rtc_date_time as defined in lib/rtc.h
 */
void ds1302_get_datetime_raw(rtc_datetime_t *dtp);

/** 
 * Fill out the passed dtp structure with current date and time, raw data is converted
 * to interger by call to ds1302_datetime_raw2int
 * @param dtp  Pointer to structure of type rtc_date_time as defined in lib/rtc.h
 */
void ds1302_get_datetime(rtc_datetime_t *dtp);

/** 
 * Convert the integer data contained in the passed dtp, to the device's raw 
 * format. 
 * Note: XXX currently only 24 hour format is supported
 * @param dtp Pointer to dtp structure containing raw data
 */
void ds1302_datetime_int2raw(rtc_datetime_t *dtp);

/** 
 * Set the date and time with data contained in the passed dtp, it contains raw
 * data in the device's register format
 * @param dtp Pointer to structure of type rtc_date_time as defined in lib/rtc.h
 */
void ds1302_set_datetime_raw(rtc_datetime_t *dtp);

/** 
 * Set the date and time with data contained in the passed dtp.
 * Note: the int data in dtp is unchanged
 * @param dtp Pointer to structure of type rtc_date_time as defined in lib/rtc.h
 */
void ds1302_set_datetime(rtc_datetime_t *dtp);

/** 
 * Write the passed command and data bytes to the ds1302
 * @param cmd The command byte
 * @param data The data byte
 */
void ds1302_write(uint8_t cmd, uint8_t data);

/** 
 * Read a byte from the device, being that returned for the passed command
 * @param cmd The command byte
 * @return The byte that has been read
 */
uint8_t ds1302_read(uint8_t cmd);


/** 
 * Control the trickle charger
 * @param cmd One of the DS1302_TX_xxx defines
 * @return The cmd byte as read from the device (should match cmd parameter)
 */
uint8_t ds1302_tc(uint8_t cmd);

#endif
