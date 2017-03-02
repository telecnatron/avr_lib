#ifndef LED_H
#define LED_H
// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2015. http://telecnatron.com/
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// -----------------------------------------------------------------------------
/**
 * @file   LED.h
 * @author Stephen Stebbing 
 * @date   Mon Dec 21 13:52:45 2015
 * 
 * @brief  Macros for controlling a single LED connected with its anode to 
 * an output pin via a current limiting resistor, and with its cathode 
 * connected to ground. This gives active-low logic.
 * 
 * Default configuration is suitable for 'Arduino Pro Mini' board with 
 * LED on port B, pin 5.
 *
 * To override the default config, define LED_DEFS prior then
 * defined the LED_XXX definitions as required.
 */

#include "config.h"


#include "../util/io.h"
#ifndef LED_DEFS
#warning Using default config. (Suitable for ProMini board LED on pin B5.)
// Defaults work with Arduino ProMini board:  LED is on pin B5.

//! Output port to which LED is connected.
#define LED_W    PORTB
//! Input port to which LED is connected.
#define LED_R    PINB
//! Pin number of the port to which LED is connected.
#define LED_PIN  PIN5
//! DDR of the port to which LED is connected.
#define LED_CTL  DDRB
#endif

//! LED control - note: active low
//! Turn LED off.
#define LED_OFF()  LED_W &=~ _BV(LED_PIN)
// Turn the LED on.
#define	LED_ON() LED_W |= _BV(LED_PIN)
//! Toggle the LED
#define LED_TOGGLE() PIN_TOGGLE(LED_W, LED_PIN)
#define PIN_TOGGLE(register, bit) (register ^= _BV(bit))
//! Initialise LED by setting LED pin as output
#define LED_INIT() LED_CTL |= _BV(LED_PIN)
#define LED_STATUS() (LED_R & _BV(LED_PIN)) ? 1: 0

#endif


