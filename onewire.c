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

#include <avr/interrupt.h>
#include <util/atomic.h>
#include <util/delay.h>
#include "onewire.h"
#include "./lib/log.h"


#ifdef ONEWIRE_USE_INTERNAL_PULLUP
// XXX use avr's internal pullup to pullup the 1-wire bus. Quite possibily THIS DOES NOT WORK.
// set pin to input. Internal pullup enabled
#define ONEWIRE_RELEASE_BUS() ONEWIRE_DDR &=~ _BV(ONEWIRE_DATA_PIN); ONEWIRE_PORT |= _BV(ONEWIRE_DATA_PIN)
#warning "Using internal pullup on 1-wire bus. XXX PLEASE CONFIRM THAT THIS ACTUALLY WORKS XXX."
#else
// set pin to input. No internal pullup enabled, external 4k7 resistor required to pull the bus data line to Vcc.
#define ONEWIRE_RELEASE_BUS() ONEWIRE_DDR &=~ _BV(ONEWIRE_DATA_PIN); ONEWIRE_PORT &=~ _BV(ONEWIRE_DATA_PIN)
#endif

// set pin to output, pull bus (data pin) low
#define ONEWIRE_SET_LO()      ONEWIRE_DDR |= _BV(ONEWIRE_DATA_PIN);  ONEWIRE_PORT &=~ _BV(ONEWIRE_DATA_PIN)


#ifdef ONEWIRE_LOGGING
#define ONEWIRE_LOG(fmt, msg...) LOG_INFO_FP(fmt, msg)
#define ONEWIRE_LOG_DEBUG(fmt, msg...) LOG_DEBUG_FP(fmt, msg)
#else
#define ONEWIRE_LOG(fmt, msg...)
#define ONEWIRE_LOG_DEBUG(fmt, msg...)
#endif

// timings in microseconds
#define ONEWIRE_DELAY_TIME_SLOT 60
#define ONEWIRE_DELAY_A 6
#define ONEWIRE_DELAY_B 64
#define ONEWIRE_DELAY_C 60
#define ONEWIRE_DELAY_D 10
#define ONEWIRE_DELAY_E 9
#define ONEWIRE_DELAY_F 55
#define ONEWIRE_DELAY_G 0
#define ONEWIRE_DELAY_H 480
#define ONEWIRE_DELAY_I 70
#define ONEWIRE_DELAY_J 410

// function declarations
static void onewire_write0();
static void onewire_write1();

void onewire_skip_rom()
{
    // Send the SKIP ROM command on the bus.
    onewire_send_byte(ONEWIRE_ROM_SKIP);
}

uint8_t onewire_receive_byte()
{
    uint8_t data=0x0;
    unsigned char i;

    // Do once for each bit
    for (i = 0; i < 8; i++)
    {
        // Shift temporary input variable right.
        data >>= 1;
        // Set the msb if a '1' value is read from the bus.
        // Leave as it is ('0') else.
        if (onewire_read_bit()){
            // Set msb
            data |= 0x80;
        }
    }
    return data;
    
}

void onewire_send_byte(uint8_t byte)
{
    uint8_t temp;
    unsigned char i;
    
    // Do once for each bit
    for (i = 0; i < 8; i++)
    {
        // Determine if lsb is '0' or '1' and transmit corresponding
        // waveform on the bus.
        temp = byte & 0x01;
        if (temp)
        {
            onewire_write1();
        }
        else
        {
	    onewire_write0();
        }
        // Right shift the data to get next bit.
        byte >>= 1;
    }
}

uint8_t onewire_detect_presence()
{
    uint8_t presenceDetected=0;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
	// set low and delay
	ONEWIRE_SET_LO();
	_delay_us(ONEWIRE_DELAY_H); // 480us
	// release and delay
	ONEWIRE_RELEASE_BUS();
	_delay_us(ONEWIRE_DELAY_I); // 70 us
	// sample bus to detect presence and delay - a present device will hold bus low
	presenceDetected = ~(ONEWIRE_PORT_IN & _BV(ONEWIRE_DATA_PIN));
	// XXX
	_delay_us(ONEWIRE_DELAY_J);
    }
    ONEWIRE_LOG_DEBUG("onewire_detect_presence() return value: %i", presenceDetected);
    return presenceDetected ;
}

void onewire_init()
{
    ONEWIRE_RELEASE_BUS();
    // The first rising edge can be interpreted by a slave as the end of a
    // Reset pulse. Delay for the required reset recovery time (H) to be 
    // sure that the real reset is interpreted correctly.
    _delay_us(ONEWIRE_DELAY_H); // 480us
}

static void onewire_write0()
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
	ONEWIRE_SET_LO();
	_delay_us(ONEWIRE_DELAY_C); // 60us
	ONEWIRE_RELEASE_BUS();
	_delay_us(ONEWIRE_DELAY_D); // 10us
    }
}

static void onewire_write1()
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
	ONEWIRE_SET_LO();
	_delay_us(ONEWIRE_DELAY_A); // 6us
	ONEWIRE_RELEASE_BUS();
	_delay_us(ONEWIRE_DELAY_B); // 64us
    }
}

uint8_t onewire_read_bit()
{
    uint8_t bit=0;
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
	// set lo and delay
	ONEWIRE_SET_LO();
	_delay_us(ONEWIRE_DELAY_A); // 6us
	// release and delay
	ONEWIRE_RELEASE_BUS();
	_delay_us(ONEWIRE_DELAY_E); // 9us
	// read bus and delay
	bit=ONEWIRE_PORT_IN & _BV(ONEWIRE_DATA_PIN);
	_delay_us(ONEWIRE_DELAY_F); // 55us
    }
    if(bit)
	return 1;
    else
	return 0;
}

uint8_t onewire_crc8(uint8_t byte, uint8_t seed)
{
   unsigned char bitsLeft;
    unsigned char temp;

    for (bitsLeft = 8; bitsLeft > 0; bitsLeft--)
    {
        temp = ((seed ^ byte) & 0x01);
        if (temp == 0)
        {
            seed >>= 1;
        }
        else
        {
            seed ^= 0x18;
            seed >>= 1;
            seed |= 0x80;
        }
        byte >>= 1;
    }
    return seed;       
}
