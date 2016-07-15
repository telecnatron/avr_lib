#ifndef DS1820_H
#define DS1820_H
// -----------------------------------------------------------------------------
// Copyright Stebbing Computing. 2013.
// $Id: ds1820.h 122 2014-09-03 04:17:38Z steves $
// -----------------------------------------------------------------------------
/**
 * @file   ds1820.h
 * @author Stephen Stebbing <steves@stebbingcomputing.com>
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

// Global variables
//! error flag - set to one of the DS1820_ERROR_XXX defines
extern uint8_t ds1820_error;

//! last read temperature raw
extern volatile int16_t ds1820_temperature_raw;
//! last read temperature
extern volatile float ds1820_temperature;

// function declarations
//! Detect a presence on the bus, return 0 (DS1820_ERROR_NONE) if presence detected, none_zero otherwise
//! Sets ds1820_error variabl
uint8_t ds1820_init();

//! Return float representation of a temperature as read by ds1820
//! ie a two's compilment, sign extended number being the temp in 16ths of a degree C
float ds1820_to_float(int16_t temp);

//! Signal the device to start convertion
uint8_t ds1820_start_conversion();

//! Start conversion, wait for completion, read result into ds1820_temperature variable.
//! XXX not sure if this actually works.
uint8_t ds1820_convert_and_read_temperature();

//! read result into ds1820_temperature variable.
uint8_t ds1820_read_temperature();

//! Read the scratchpad and calculate the crc.
// Return true if the calculated crc matches the crc stored in the scratchpad, false otherwise.
// XXX note: this function has not been tested.
uint8_t ds1820_verify_scratchpad_crc();
#endif
