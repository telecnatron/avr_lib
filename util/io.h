#ifndef IO_H
#define IO_H
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
#include <avr/io.h>

//! toggle the passed bit number  of the passed register
#define PIN_TOGGLE(register, bit) (register ^= _BV(bit))
//! set the passed bit number of the passed register to high
#define PIN_HI(register,bit)      (register |= _BV(bit))
//! set the passed bit number of the passed register to low
#define PIN_LO(register,bit)      (register &=~ _BV(bit))

#define BIT_HI(byte, bit) PIN_HI(byte, bit)
#define BIT_LO(byte, bit) PIN_LO(byte, bit)
#define BIT_TOGGLE( byte, bit) PIN_TOGGLE( byte, bit)
//! Return non-zero if passed bit number is set in the passed byte
#define BIT_IS_SET(byte,bit_num) (byte & _BV(bit_num))


#endif
