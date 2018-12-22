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
#include "ds1820.h"
//#include "onewire.h"


// memory commands
#define DS1820_START_CONVERSION 0x44
#define DS1820_WRITE_SCRATCH    0x4e
#define DS1820_READ_SCRATCH     0xbe
#define DS1820_COPY_SCRATCH     0x48
#define DS1820_RECALL           0xb8
#define DS1820_READ_POWER       0xb4

#ifdef DS1820_LOGGING
#include "lib/log.h"
#define DS1820_LOG(fmt, msg...) LOG_INFO_FP(fmt, msg)
#define DS1820_LOG_ERROR(fmt, msg...) LOG_INFO_FP(fmt, msg)
#define DS1820_LOG_DEBUG(fmt, msg...) LOG_DEBUG_FP(fmt, msg)
#else
#define DS1820_LOG(fmt, msg...)
#define DS1820_LOG_DEBUG(fmt, msg...)
#define DS1820_LOG_ERROR(fmt, msg...)
#endif



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

uint8_t ds1820_convert_and_read_temperature(float *temp)
{
    if(ds1820_start_conversion()){
	return 1;
    }
    // wait for conversion to complete - ds1820 holds bus low while conversion in progress,
    // this should be ~1ms
    while(!onewire_read_bit()){}
    // read temp from device
    if(ds1820_read_temperature(temp)){
	return 2;
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

uint8_t ds1820_read_temperature(float *temp)
{
    int16_t temp_raw;

    // read temperate from the scratch pad
    //  reset, presence
    if(!onewire_detect_presence()){
	return 1;
    }
    // read result
    onewire_skip_rom();
    onewire_send_byte(DS1820_READ_SCRATCH);
    // lsb
    temp_raw = onewire_receive_byte();
    // msb
    temp_raw |= (onewire_receive_byte() << 8);
    // convert to float
    *temp=DS1820_RAW_TO_FLOAT(temp_raw);
    // done - note that we only read the first two bytes from scratchpad - that's all we need here
    return 0;
}



