#ifndef ONEWIRE_H
#define ONEWIRE_H
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
 * Interface/control for Dallas Semicondutor One-Wire bus.
 *
 * Note: interrupts are potentially disabled while bus transactions are occuring. This may
 * be for a period of 1ms or more.
 *
 * Note: current implements allows for only one slave device on the bus.
 *
 * See http://www.atmel.com/images/doc2579.pdf
 */
#include "config.h"
#include <avr/io.h>

// io pins
#ifndef ONEWIRE_DEFS
#define ONEWIRE_DEFS
#warning "onewire.h is using default HW definitions"
#define ONEWIRE_PORT PORTC
#define ONEWIRE_PORT_IN  PINC
#define ONEWIRE_DDR  DDRC
#define ONEWIRE_DATA_PIN  PIN1
#undef  ONEWIRE_USE_INTERNAL_PULLUP
#endif //ifndef ONEWIRE_DEFS

// rom commands
#define     ONEWIRE_ROM_READ    0x33    //!< READ ROM command code.
#define     ONEWIRE_ROM_SKIP    0xcc    //!< SKIP ROM command code.
#define     ONEWIRE_ROM_MATCH   0x55    //!< MATCH ROM command code.
#define     ONEWIRE_ROM_SEARCH  0xf0    //!< SEARCH ROM command code.

// function declarations
//!
void onewire_init();
//! returns true if one or more devices on the bus indicate as being present.
uint8_t onewire_detect_presence();
//!
void onewire_send_byte(uint8_t byte);
//!
uint8_t onewire_receive_byte();
//! Send the skip rom command
void onewire_skip_rom();
//! Read a single bit 
uint8_t onewire_read_bit();

// calculate 8-bit ds onewire crc 
uint8_t onewire_crc8(uint8_t byte, uint8_t seed);

#endif
