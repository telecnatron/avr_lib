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
 * @brief  Microcontroller Common User Interface LED control.
 * 
 * Provides for up to 8 LEDS to be controlled individually by pusle width modulating
 * their brightness, blinking them with variable duty cycle, and with a fixed-number 
 * or infinite number of blinks .
 *
 * HW pin assignments are read from mcui.defs file which is created with the help of
 * a utility program: mcui.py
 * 
 * See http://telecnatron.com/articles/mcui for further details.
 */

#include <avr/io.h>
#include <stdint.h>
#include "mcui.defs"

// Definitions
//! Number of bits used for setting LED brightness
#define MCUI_LED_PWM_NUM_BITS 10
#define MCUI_LED_BRIGHTNESS_MAX MCUI_LED_PWM_NUM_BITS - 1

//! LED control structure
typedef struct {
    //! brightness: range 0 to MCUI_LED_PWM_NUM_BITS-1. 0 being always off, MCUI_LED_PWM_NUM_BITS-1 being always on
    uint8_t brightness;
    //! blinky variables:
    //! total number of ticks in one cycle
    uint16_t ticks_max;
    //! tick count at which led is turned off
    uint16_t ticks_off;
    //! current tick count
    uint16_t ticks;
    //! number of repeat-cycles remaining
    uint8_t cycles;
} mcui_led_ctrl_t;

//! Global array of control structures, 1 for each LED
extern mcui_led_ctrl_t mcui_led_ctrl[MCUI_NUM_LED];

//! Initialise LED pins and turn LEDs off.
void mcui_led_init();

/** 
 * Enable LED with passed index. This allows for the LED to be turned on
 * by call to mcui_led_tick() if blinking and pwm require for it to be.
 * 
 * @param led Index of the LED to be enabled
 */
void mcui_led_enable(uint8_t led);

/** 
 * Disable LED with passed index. If disabled the LED will never be turned
 * on by calls to mcui_led_tick(), regardless of pwm and blinky settings.
 *
 * @param led Index of the LED to be enabled
 */
void mcui_led_disable(uint8_t led);

//! Toggle disable/enable for LED with passed index.
void mcui_led_toggle(uint8_t led);

/** 
 * Increment timers and turn individual LEDS on or off as required by
 * current PWM and blinky settings. This function needs to be called periodically
 * at the PWM clock rate.
 */
void mcui_led_tick();

/** 
 * Set LED brigtness
 * 
 * @param led index of the LED that is to be set: 0 to (MCUI_NUM_LED - 1)
 * @param bright the brightness level to bet set. 0 to MCUI_LED_BRIGHTNESS_MAX
 */
void mcui_led_set_brightness(uint8_t led, uint8_t bright);

/** 
 * Change the LED's brigtness setting by the passed amount (ie -ve value will decrease brightness)
 * Function ensures the resulting brightness setting is always within the range 0 to MCUI_LED_BRIGHTNESS_MAX
 * 
 * @param led Index of the LED that is to have its brightness incremented.
 * @param inc The amount to increment by. May be -ve.
 */
void mcui_led_inc_brightness(uint8_t led, int8_t inc);

/** 
 * Set the LED to be blinking. A cycle consists of the LED being on for ticks_on ticks,
 * and then off for ticks_off ticks. Hence period of a cycle is ticks_on + ticks_off ticks.
 * 
 * @param led Index of the LED that is to be set.
 * @param ticks_on Number of ticks within a cycle that the LED is to be on for
 * @param ticks_off Number of ticks within a cycle that the LED is to be off for.
 * @param cycles The number of times that the blinking will repeat. Use 0 for infinite.
 */
void mcui_led_set_blinky(uint8_t led, uint16_t ticks_on, uint16_t ticks_off, uint8_t cycles);


//!
void mcui_led_set_brightness(uint8_t led, uint8_t brightness);
//!
uint8_t mcui_led_get_brightness(uint8_t led);
//!
void mcui_led_set_period(uint8_t led, uint16_t period_ticks);
//!
uint16_t mcui_led_get_period(uint8_t led);
//!
void mcui_led_set_duty_cycle(uint8_t led, uint16_t duty_cycle_ticks);
//!
uint16_t mcui_led_get_duty_cycle(uint8_t led);

#endif /* _MCUI_LED_H */

