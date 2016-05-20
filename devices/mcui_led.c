// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2016. http://telecnatron.com/
// -----------------------------------------------------------------------------
#include "mcui_led.h"

//! LED control structure
typedef struct {
    // brightness: range 0-8. 0 being always off, 8 being always on
    uint8_t pwm_max_count;
} mcui_led_ctrl_t;

//! array of control structures, 1 for each LED
mcui_led_ctrl_t mcui_led_ctrl[MCUI_NUM_LED];
//!
static uint8_t pwm_count;

void mcui_led_init()
{
    MCUI_INIT_LED();
}


void mcui_led_tick()
{
    pwm_count++;
    // mod 8
    pwm_count = pwm_count % 8;

    for(int i = 0; i < MCUI_NUM_LED; i++){
	mcui_led_ctrl_t *lc = &(mcui_led_ctrl[i]);
	if(lc->pwm_max_count < pwm_count){
	    // led should be on
	}else{
	    // led should be off
	}
    }
    
}
