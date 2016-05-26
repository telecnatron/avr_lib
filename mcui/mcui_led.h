#ifndef _MCUI_LED_H
#define _MCUI_LED_H 1
// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2016. http://telecnatron.com/
// -----------------------------------------------------------------------------
/**
 * @file   mcui_led.h
 * @author steves
 * @date   2016/05/20 04:46:11
 * 
 * @brief  
 * 
 */
#include <avr/io.h>
#include <stdint.h>
#include "mcui.defs"


// Definitions
//! Number of bits used for setting LED brightness
#define MCUI_LED_PWM_NUM_BITS 10
#define MCUI_LED_BRIGHTNESS_MAX MCUI_LED_PWM_NUM_BITS - 1


//! Initialise LED pins and turn LEDs off.
void mcui_led_init();

//!
void mcui_led_enable(uint8_t led);
//!
void mcui_led_disable(uint8_t led);
//!
void mcui_led_toggle(uint8_t led);



//! 
void mcui_led_tick();

//!
/** 
 * Set LED brigtness
 * 
 * @param led index of the LED that is to be set: 0 to (MCUI_NUM_LED - 1)
 * @param bright the brightness level to bet set. 0 to MCUI_LED_BRIGHTNESS_MAX
 */
void mcui_led_set_brightness(uint8_t led, uint8_t bright);

//!
void mcui_led_inc_brightness(uint8_t led, int8_t inc);

//!
void mcui_led_set_blinky(uint8_t led, uint16_t ticks_on, uint16_t ticks_off, uint8_t cycles);




#endif /* _MCUI_LED_H */

