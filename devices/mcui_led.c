// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2016. http://telecnatron.com/
// -----------------------------------------------------------------------------
#include "mcui_led.h"

// XXX debug
#include <stdio.h>

//! LED control structure
typedef struct {
    // brightness: range 0-MCUI_LED_PWM_NUM_BITS. 0 being always off, MCUI_LED_PWM_NUM_BITS being always on
    uint8_t pwm_max_count;

    // blinky variables:
    // total number of ticks in one cycle
    uint16_t ticks_max;
    // tick count at which led is turned off
    uint16_t ticks_off;
    // current tick count
    uint16_t ticks;
    // number of repeat-cycles remaining
    uint8_t cycles;
} mcui_led_ctrl_t;

//! array of control structures, 1 for each LED
mcui_led_ctrl_t mcui_led_ctrl[MCUI_NUM_LED];
//! current value of pwm counter
static volatile uint8_t mcui_led_pwm_count;
//! master enable bit-mask. If bit is set then corresponding LED will be turned on.
//! note: initial configuration is for all to be enabled
volatile uint8_t mcui_led_mask_enable = (_BV(MCUI_NUM_LED) -1);
//!
volatile uint8_t mcui_led_mask_blinky=0;
volatile uint8_t mcui_led_mask_blinky_repeat=0;

//!
inline void mcui_led_init()
{
    MCUI_INIT_LED();
}

void mcui_print_debug(uint8_t led)
{
    mcui_led_ctrl_t *lc = &(mcui_led_ctrl[led]);
    printf("led: %u, tm: %u, to: %u, t: %u, c: %u, me: 0x%0x2, mb: 0x%02x, mbr: 0x%02x,\n", led, lc->ticks_max, lc->ticks_off, lc->ticks, lc->cycles, mcui_led_mask_enable, mcui_led_mask_blinky, mcui_led_mask_blinky_repeat);
}

void mcui_led_set_blinky(uint8_t led, uint16_t ticks_on, uint16_t ticks_off, uint8_t cycles)
{
    mcui_led_ctrl_t *lc = &(mcui_led_ctrl[led]);
    lc->ticks_max = ticks_on + ticks_off;
    lc->ticks_off = ticks_on;
    mcui_led_mask_blinky |= _BV(led);
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
}

//! Apply enable mask and turn leds on accordingly
static inline void mcui_led_set_leds(uint8_t leds)
{
    MCUI_WRITE_LED(leds & mcui_led_mask_enable);
}

inline void mcui_led_brightness(uint8_t led, uint8_t pwm)
{
    mcui_led_ctrl[led].pwm_max_count=pwm % MCUI_LED_PWM_NUM_BITS;
}

void mcui_led_tick()
{
    // bit field for leds 
    uint8_t ledsb=0;
    uint8_t ledsp=0;

    // loop thru leds
    for(int i = 0; i < MCUI_NUM_LED; i++){

	// do blinky
	if( mcui_led_mask_blinky & _BV(i)){
	    mcui_led_ctrl_t *lc = &(mcui_led_ctrl[i]);
	    if(lc->cycles ){
		// yup, LED is blinky
		lc->ticks++;
		if( lc->ticks <= lc->ticks_off){
		    // led should be on
		    ledsb |= _BV(i);
		}else{
		    // led should be off
		    ledsb &= ~_BV(i);
		}
		if( lc->ticks == lc->ticks_max)
		{
		    // cycle completed
		    lc->ticks = 0;
		    if( !(mcui_led_mask_blinky_repeat & _BV(i)) ){
			//  not repeating 
			lc->cycles--;
		    }
		    if( lc->cycles ){
			// cycles remain, turn led back on
			ledsb |= _BV(i);
		    }else{
			// cycles completed, turn off
			ledsb &= ~_BV(i);
			// deassert blinky bit
			mcui_led_mask_blinky &= ~_BV(i);
		    }
		}
	    }
	}

	// do brightness pwm
	// pwm counter runs mod 8
	mcui_led_pwm_count++;
	mcui_led_pwm_count = mcui_led_pwm_count % MCUI_LED_PWM_NUM_BITS;

	if( mcui_led_ctrl[i].pwm_max_count > mcui_led_pwm_count ){
	    // led should be on, set its bit
	    ledsp |= _BV(i);
	}else{
	    // led should be off, clear its bit
	    ledsp &= ~_BV(i);
	}
    }

    // turn leds on/off
    mcui_led_set_leds(ledsb & ledsp);
   
}
