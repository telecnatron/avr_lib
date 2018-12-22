#ifndef DHT11_H
#define DHT11_H
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
//
// Interface to dht11 humidity and temperature sensor
//
// Device has 4 pins.
// Pin one is on the left when device has square grid facing towards you
// 1: VCC - 3.5 to 5.5v
// 2: DATA
// 3: NC
// 4: GND
//
// Note: 
//   * Device should not be read from at least 2 seconds after startup
//   * Mininum of 5 seconds between reads
//   * A timer is required - this is configured via the DHT11_TIMER_xxx defines below.
// -----------------------------------------------------------------------------
#include "config.h"
#include <avr/io.h>
#include "../util/io.h"
// -----------------------------------------------------------------------------
#ifndef DHT11_PIN_DEFS
#define DHT11_PIN_DEFS
#warning "Using default defs"
// device definitions. If these are overriden prior, be sure to defined DHT11_DEFS
// device connected on PINC3, pin 26 on dip package
#define DHT11_DDR       DDRC
#define DHT11_PORT_OUT  PORTC
#define DHT11_PORT_IN   PINC
#define DHT11_PIN       PIN3
#endif

#ifndef DHT11_DEFS
#define DHT11_DEFS
// timer setup stuff
// assumes F_CPU=16M, uses timer0 on atmega328, 168, 88, 8
#if F_CPU==16000000
// timer current count register
#define DHT11_TIMER_COUNT TCNT0
// init timer. Normal operation, prescaller divide by 8, gives 2 counts per microsecond at 16MHz CPU 
#define DHT11_TIMER_INIT() TCCR0A=0; TCCR0B=0; TCCR0B|=_BV(CS21);
// reset count to 0
#define DHT11_TIMER_RESET() DHT11_TIMER_COUNT=0

// timeout values in timer ticks - required usec plus a margin
#define DHT11_TIMER_COUNT_40US 90
#define DHT11_TIMER_COUNT_50US 120
#define DHT11_TIMER_COUNT_70US 170 
#define DHT11_TIMER_COUNT_80US 200 

#define DHT11_TIMER_COUNT_BIT0_MIN 40
#define DHT11_TIMER_COUNT_BIT0_MAX 70
#define DHT11_TIMER_COUNT_BIT1_MIN 120
#define DHT11_TIMER_COUNT_BIT1_MAX 160
#endif
#else
#error "Please configure timing counts for this F_CPU frequency"
#endif // F_CPU

// -----------------------------------------------------------------------------
// error codes
#define DHT11_ERROR_OK          0
#define DHT11_ERROR_NO_RESPONSE_1 1
#define DHT11_ERROR_NO_RESPONSE_2 2
#define DHT11_ERROR_NO_RESPONSE_3 3
#define DHT11_ERROR_BIT_READ_1    4
#define DHT11_ERROR_BIT_READ_2    5
#define DHT11_ERROR_BIT_READ_3    6
#define DHT11_ERROR_CHECKSUM      7


// data type for reading
typedef struct {
    uint8_t humidity;
    uint8_t temp;
} dht11_data_t;

/** 
 * Read the device.
 * Note: 
 *    * Should not be called more frequently than at 5 second interval.
 * @param reading 4 bytes read from the device. MSByte is humidity int, next humidity decimal, temp int, temp decimal
 * @return 0 on success, non-zero otherwise
 */
uint8_t dht11_read(dht11_data_t *reading);

#endif
