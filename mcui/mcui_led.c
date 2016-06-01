// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2016. http://telecnatron.com/
// -----------------------------------------------------------------------------
/**
 * @file   mcui_led.c
 * @author Stephen Stebbing <steves@slap.ss.com>
 * @date   Fri May 27 12:57:29 2016
 * 
 * @brief  Microcontroller Common User Interface LED control
 * 
 * 
 */

#include "mcui_led.h"

//! array of control structures, 1 for each LED
mcui_led_ctrl_t mcui_led_ctrl[MCUI_NUM_LED];

//! current value of pwm counter
static volatile uint8_t mcui_led_pwm_count=0;

//! master enable bit-mask. If bit is set then corresponding LED will be turned on when necessary.
//! note: initial configuration is for all to be enabled
volatile uint8_t mcui_led_mask_enable = (_BV(MCUI_NUM_LED) -1);

//! Bit set for LEDs that are currenly blinking
volatile uint8_t mcui_led_mask_blinky;
//! Bit fields indicates whether blinking LEDs repeat
volatile uint8_t mcui_led_mask_blinky_repeat;

//!
inline void mcui_led_init()
{
    MCUI_INIT_LEDS();
    // disabled
    mcui_led_mask_enable = 0;
}

inline void mcui_led_enable(uint8_t led)
{
    mcui_led_mask_enable |= _BV(led);
}

inline void mcui_led_disable(uint8_t led)
{
    mcui_led_mask_enable &= ~_BV(led);
}

inline void mcui_led_toggle(uint8_t led)
{
    mcui_led_mask_enable ^= _BV(led);
}

void mcui_led_inc_brightness(uint8_t led, int8_t inc)
{
    int8_t nb =  mcui_led_ctrl[led].brightness + inc;
    if(nb < 0 )
	nb=0;
    else if (nb > MCUI_LED_BRIGHTNESS_MAX)
	nb = MCUI_LED_BRIGHTNESS_MAX;
    mcui_led_set_brightness(led, nb);
}

void mcui_led_set_blinky(uint8_t led, uint16_t ticks_on, uint16_t ticks_off, uint8_t cycles)
{
    mcui_led_ctrl_t *lc = &(mcui_led_ctrl[led]);
    lc->ticks_max = ticks_on + ticks_off;
    lc->ticks_off = ticks_on;
    lc->ticks = 0;
    if( cycles == 0 ){
	// repeat indefinitely
	lc->cycles = 1;
	// set repeat bit for this led
	mcui_led_mask_blinky_repeat |= _BV(led);
    }else{
	// repeat for fixed number of cycles
	lc->cycles = cycles;
	// clear repeat bit for this led
	mcui_led_mask_blinky_repeat &= ~_BV(led);
    }
    // flag LED as being blinky
    mcui_led_mask_blinky |= _BV(led);
}


void mcui_led_tick()
{
    // increment pwm counter, runs mod MCUI_LED_PWM_NUM_BITS
    mcui_led_pwm_count++;
    mcui_led_pwm_count = mcui_led_pwm_count % MCUI_LED_PWM_NUM_BITS;
    

    // bit mask for leds, 
    // here we assume that they should be all ON,
    // below we turn them off as required.
    uint8_t leds = 0xff;

    for(int i = 0; i < MCUI_NUM_LED; i++){

	mcui_led_ctrl_t *lc = &(mcui_led_ctrl[i]);

	// do blinkying
	if(mcui_led_mask_blinky & _BV(i)){
	    // yup, led is blinkying
	    lc->ticks++;

	    if( ! (lc->ticks < lc->ticks_off) ){
		// led should be off, make it so
		leds &= ~_BV(i);
	    }

	    if( lc->ticks == lc->ticks_max)
	    {
		// cycle completed
		lc->ticks = 0;
		if( !(mcui_led_mask_blinky_repeat & _BV(i)) ){
		    //  not repeating 
		    lc->cycles--;
		}
		if( ! lc->cycles ){
		    // cycles completed, turn off
		    leds &= ~_BV(i);
		    // and disable
		    mcui_led_disable(i); 
		    // clear blinky mask
		    mcui_led_mask_blinky &= ~_BV(i);
		}
	    }
	}

	// do brightness pwm.
	if( lc->brightness <= mcui_led_pwm_count ){
	    // led should be off, clear its bit
	    leds &= ~_BV(i);
	}

    }    

    // apply enable mask and turn leds on/off
    MCUI_WRITE_LEDS(leds & mcui_led_mask_enable);
   
}

inline void mcui_led_set_brightness(uint8_t led, uint8_t bright)
{
    mcui_led_ctrl[led].brightness = bright;
}

inline uint8_t mcui_led_get_brightness(uint8_t led)
{
    return mcui_led_ctrl[led].brightness;
}

inline void mcui_led_set_period(uint8_t led, uint16_t period_ticks)
{
    mcui_led_ctrl[led].ticks_max = period_ticks;
}

inline uint16_t mcui_led_get_period(uint8_t led)
{
    return mcui_led_ctrl[led].ticks_max;
}

inline void mcui_led_set_duty_cycle(uint8_t led, uint16_t duty_cycle_ticks)
{
    mcui_led_ctrl[led].ticks_off= duty_cycle_ticks;
}

inline uint16_t mcui_led_get_duty_cycle(uint8_t led)
{
    return mcui_led_ctrl[led].ticks_off;
}

