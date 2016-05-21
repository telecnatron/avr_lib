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

// Macros
#define MCUI_LED_ENABLE_LED(led_num)  (mcui_led_mask_enable |= _BV(led_num))
#define MCUI_LED_DISABLE_LED(led_num) (mcui_led_mask_enable &= ~_BV(led_num))
#define MCUI_LED_BLINKY_ENABLE_LED(led_num)  (mcui_led_mask_blinky |= _BV(led_num))
#define MCUI_LED_BLINKY_DISABLE_LED(led_num) (mcui_led_mask_blinky &= ~_BV(led_num))


// Globals
// led enable mask. If bit is set then corresponding LED is enabled
extern volatile uint8_t mcui_led_mask_enable;
//! blinky mask. If bit is set the corresponding LED is able to be blinkied.
extern volatile uint8_t mcui_led_mask_blinky;


//! Initialise LED pins and turn LEDs off.
void mcui_led_init();

//!
void mcui_led_set_blinky(uint8_t led, uint16_t ticks_on, uint16_t ticks_off, uint8_t cycles);
//! 
void mcui_led_tick();


/** 
 * Set LED brigtness
 * 
 * @param led index of the LED that is to be set: 0 to (MCUI_NUM_LED - 1)
 * @param pwm the brightness level to bet set. 0 to MCUI_LED_PWM_NUM_BITS-1
 */
void mcui_led_brightness(uint8_t led, uint8_t pwm);

#endif /* _MCUI_LED_H */

