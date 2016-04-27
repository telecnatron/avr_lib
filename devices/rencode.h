#ifndef RENCODE_H
#define RENCODE_H
// -----------------------------------------------------------------------------
// Copyright Stebbing Computing. 2015.
// $Id: rencode.h 326 2015-09-28 21:18:17Z steves $
//
// Rotary encoder library.
//
// Uses algorithm as described here: 
// https://www.circuitsathome.com/mcu/rotary-encoder-interrupt-service-routine-for-avr-micros
//
// Current implementation uses pin-change interrupt, and hence unavailable on eg ATMEGA8
// -----------------------------------------------------------------------------
#include <stdint.h>
#include "config.h"

#ifndef RE_DEFS
#warning "rencode.h is using default HW definitions"
// Rotary encoder setup
#define RE_W PORTC
#define RE_R PINC
#define RE_CTL  DDRC
// C0 is PCINT8
#define RE_PIN0 PIN0
// C1 is PCINT9
#define RE_PIN1 PIN1
// this is the pin-change interrupt bit for PCICR register
// PCIE1 corresponds to PCINT8-14 pins
#define RE_PCINT PCIE1
// interrupt vector name for PCINT
#define RE_ISR_NAME PCINT1_vect
// pin change mask regiser, pcint8 and 9 are controlled by PCMSK1
#define RE_PCMSK_REG PCMSK1
#endif

//! init rotary encoder pins as inputs, PUs enabled. Enables PCINT
//! re_handler_fn is pointer thats gets called by ISR when a click is detected.
//! dir indicates direction: 0 or 1
//! Note: handler function is called from within the ISR
void re_init(void (* re_handler_fn)(uint8_t dir));

//! Rotary encoder state machine. Should be called from ISR(RE_ISR_NAME) routine which needs to be user-provided.
//! see https://www.circuitsathome.com/mcu/rotary-encoder-interrupt-service-routine-for-avr-micros
void re_isr_handler();

#endif
