#ifndef ONEWIRE_H
#define ONEWIRE_H
// -----------------------------------------------------------------------------
// Copyright Stebbing Computing. 2013.
// $Id: onewire.h 122 2014-09-03 04:17:38Z steves $
// -----------------------------------------------------------------------------
/** 
 * Interface/control for Dallas Semicondutor One-Wire bus.
 *
 * Note: interrupts are potentially disabled while bus transactions are occuring. This may
 * be for a period of 1ms or more.
 *
 */
#include "config.h"
#include <avr/io.h>

// io pins
#ifndef ONEWIRE_DEFS
#warning "onewire.h is using default HW definitions"
#define ONEWIRE_PORT PORTD
#define ONEWIRE_PORT_IN  PIND
#define ONEWIRE_DDR  DDRD
#define ONEWIRE_DATA_PIN  PIN7
#endif //ifndef ONEWIRE_DEFS

// rom commands
#define     ONEWIRE_ROM_READ    0x33    //!< READ ROM command code.
#define     ONEWIRE_ROM_SKIP    0xcc    //!< SKIP ROM command code.
#define     ONEWIRE_ROM_MATCH   0x55    //!< MATCH ROM command code.
#define     ONEWIRE_ROM_SEARCH  0xf0    //!< SEARCH ROM command code.

// function declarations
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
