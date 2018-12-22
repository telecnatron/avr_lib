#ifndef _INT0_H
#define _INT0_H 1
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
 * @file   int0.h
 * @author steves
 * @date   2016/04/19 11:27:26
 * 
 * @brief  Macros for external interrupt INT0.
 * ISR handler is defined as:
 *   ISR(INT0_vect){...}
 */
// -------------------------------------------------------------
// atmega xx8 ...
#if defined(ATMEGA328) || defined(ATMEGA168) || defined(ATMEGA88)

// enable/disable INT0 interrupt.
#define INT0_ENABLE()    EIMSK |= _BV(INT0)
#define INT0_DISABLE()   EIMSK &=~ _BV(INT0)

// pull up resistor enable/disable
#define INT0_PU_ENABLE()  PORTD |=  _BV(PIN2)
#define INT0_PU_DISABLE() PORTD &=~ _BV(PIN2)

// init key - on INT0, PD2, pin 4 of DIL.
// Set D2 as input and enable pullup
#define INT0_INIT()  DDRD &=~ _BV(PIN2); INT0_PU_ENABLE()

// interrupt trigger senses.
// trigger on low level
#define INT0_SENSE_LO()   EICRA &=~ (_BV(ISC01) | _BV(ISC00))
// trigger on any logic change
#define INT0_SENSE_ANY()  EICRA &=~ _BV(ISC01); EICRA |= _BV(ISC00)
// trigger on falling edge
#define INT0_SENSE_FALL() EICRA |=  _BV(ISC01); EICRA &=~ _BV(ISC00)
// trigger on rising edge
#define INT0_SENSE_RISE() EICRA |=  _BV(ISC01)  | _BV(ISC00)

// read state of int0 pin
#define INT0_READ() (PIND & _BV(PIN2))
// -------------------------------------------------------------
// ... other processors
#else
#error "INT0 is not defined for this processor."
#endif
// -------------------------------------------------------------
#endif /* _INT0_H */

