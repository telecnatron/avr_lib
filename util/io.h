#ifndef IO_H
#define IO_H
// -----------------------------------------------------------------------------
// Copyright Stebbing Computing. 2013.
// $Id: io.h 403 2015-12-27 03:11:36Z steves $
// -----------------------------------------------------------------------------
#include <avr/io.h>

//! toggle the passed bit number  of the passed register
#define PIN_TOGGLE(register, bit) (register ^= _BV(bit))
//! set the passed bit number of the passed register to high
#define PIN_HI(register,bit)      (register |= _BV(bit))
//! set the passed bit number of the passed register to low
#define PIN_LO(register,bit)      (register &=~ _BV(bit))

#define BIT_HI(byte,bit) PIN_HI(byte,bit)
#define BIT_LO(byte,bit) PIN_LO(byte,bit)
//! Return non-zero if passed bit number is set in the passed byte
#define BIT_IS_SET(byte,bit_num) (byte & _BV(bit_num))


#endif
