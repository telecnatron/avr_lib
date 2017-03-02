// -----------------------------------------------------------------------------
// Copyright Stebbing Computing. 2017. http://telecnatron.com/
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// -----------------------------------------------------------------------------
#include "ds1820.h"
//#include "onewire.h"
#include "./lib/log.h"

// last read temperature - in 16ths of a degree C
volatile int16_t ds1820_temperature_raw;
// last read temperature as float
volatile float   ds1820_temperature;

// memory commands
#define DS1820_START_CONVERSION 0x44
#define DS1820_WRITE_SCRATCH    0x4e
#define DS1820_READ_SCRATCH     0xbe
#define DS1820_COPY_SCRATCH     0x48
#define DS1820_RECALL           0xb8
#define DS1820_READ_POWER       0xb4


inline float ds1820_to_float(int16_t temp)
{
    return (temp/16.0);
}

uint8_t ds1820_verify_scratchpad_crc()
{
    uint8_t crc=0;
    // read first 8 bytes from the scratch pad, calculating their crc
    onewire_skip_rom();
    onewire_send_byte(DS1820_READ_SCRATCH);
    int i;
    for(i=0; i<8; i++){
	crc=onewire_crc8(onewire_receive_byte(),crc);
    }
    // read 9th byte and verify that it matches the calculated crc
    return onewire_receive_byte() == crc;
}



uint8_t ds1820_init()
{
    onewire_init();
    return onewire_detect_presence();
}

uint8_t ds1820_convert_and_read_temperature()
{
    if(!ds1820_start_conversion()){
	return 1;
    }
    // wait for conversion to complete - ds1820 holds bus low while conversion in progress
    while(!onewire_read_bit()){}
    // read temp from device
    if(ds1820_read_temperature()){
	return 1;
    }
    return 0;
}

uint8_t ds1820_start_conversion()
{
    if(!onewire_detect_presence()){
	return 1;
    }
    // we assume that there is only one device on the bus, therefore skip rom will address it
    onewire_skip_rom();
    onewire_send_byte(DS1820_START_CONVERSION);
    return 0;
}

uint8_t ds1820_read_temperature()
{
    // read temperate from the scratch pad
    //  reset, presence
    if(!onewire_detect_presence()){
	return 1;
    }
    // read result
    onewire_skip_rom();

    onewire_send_byte(DS1820_READ_SCRATCH);
    // lsb
    ds1820_temperature_raw = onewire_receive_byte();
    // msb
    ds1820_temperature_raw |= (onewire_receive_byte() << 8);

    // convert to float
    ds1820_temperature=DS1820_RAW_TO_FLOAT(ds1820_temperature_raw);
    // done - we only read the first two bytes from scratchpad - that's all we need here
    return 0;
}



