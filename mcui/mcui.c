// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2016. http://telecnatron.com/
// $Id: $
// -----------------------------------------------------------------------------
#include "mcui.h"
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

//! current state of buttons
uint8_t mcui_btn_state;
//! for debouncing
static uint8_t mcui_btn_prev_state;

//! ticker, used for timings, is incremented by call to mcui_tick
uint16_t mcui_ticks;
//! array of click-times. One for each button
uint16_t mcui_btn_click_time[MCUI_NUM_PB];
//! buttons click state: 0 indicates waiting for down, 1 indicates waiting for up.
uint8_t mcui_btn_click_state;
//! rotary encoder turns: -ve indicates CCW, +ve CW
int8_t mcui_re_turns;

// handler functions
void (*mcui_click_handler)(uint8_t btn);
void (*mcui_changed_handler)(uint8_t btn_changed, uint8_t btn_state);
void (*mcui_re_turned_handler)(int8_t dir);

void mcui_init(void (*changed_fn), void (*click_fn), void(*re_fn))
{
    MCUI_INIT_PB();
    MCUI_INIT_RE();
    mcui_click_handler = click_fn;
    mcui_changed_handler = changed_fn;
    mcui_re_turned_handler= re_fn;
}

inline uint16_t mcui_get_ticks()
{
    return mcui_ticks;
}

//! Reads the rotary encoder's pins, returns
//! a uint8_t with bit0 set to 1 if RE_PIN0 is asserted, 0 otherwise,
//! and bit1 set to 1 if RE_PIN1 is asserted, 0 otherwise.
//! eg 0x3 indicates that both pins are asserted.
static inline uint8_t mcui_re_read()
{
    return MCUI_READ_RE_0() | (MCUI_READ_RE_1() << 1);
}

//! Rotary encoder state machine. Should be called from the PCINTx_vect ISRs for the pins that RE is connected to.
//! See https://www.circuitsathome.com/mcu/rotary-encoder-interrupt-service-routine-for-avr-micros
void mcui_re_isr()
{
    // previous index into lookup table
    static uint8_t re_prev = 3;
    // encoder value
    static int8_t re_value = 0;
    // state table
    static const int8_t re_states [] PROGMEM = 
	{0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};  //encoder lookup table

    //remember previous state - shift it 2 bits right
    re_prev <<=2;  
    // append current value to last 2 lsbits
    re_prev |= mcui_re_read();
    // lookup up increment value in state table
    re_value += pgm_read_byte(&(re_states[( re_prev & 0x0f )]));

    if(re_value > 3){
	if(mcui_re_turns < INT8_MAX)
	    mcui_re_turns++;
	re_value=0;
    }else  if(re_value < -3){
	if(mcui_re_turns > INT8_MIN)
	    mcui_re_turns--;
	re_value=0;
    }
}


inline uint8_t mcui_read_btns()
{
    return mcui_btn_state;
}

inline void mcui_detect_click(uint8_t changed_flags, uint8_t btns)
{
    // loop thru each button
    for(uint8_t i=0; i < MCUI_NUM_PB; i++){
	if( changed_flags & _BV(i)){
	    // button has changed
	    if( !(mcui_btn_click_state & _BV(i)) ){
		// looking for down (asserted)
		if(btns & _BV(i)){
		    // yup, it's down alright.
		    // record time
		    mcui_btn_click_time[i]=mcui_ticks;
		    // set bit to indicate that now looking for up
		    mcui_btn_click_state |= _BV(i);
		}
	    }else if(  !(btns & _BV(i)) ){
		// yup, button is up (unasserted)
		// check down time is less than max
		if( (mcui_ticks - mcui_btn_click_time[i]) < MCUI_CLICK_TICKS){
		    // call click handler
		    if(mcui_click_handler)
			mcui_click_handler(i);
		}
		// clear bit to indicate that now looking for down
		mcui_btn_click_state &= ~_BV(i);
	    }
	}
    }
}

void mcui_tick()
{
    mcui_ticks++;

    // bits indicating which buttons have changed state
    uint8_t changed_flags=0;

    // state of buttons as now read from pins
    uint8_t buts=0;
    MCUI_READ_PB(buts);

    // debounce by checking if they were the same as last time...
    if(buts == mcui_btn_prev_state){
	// ... yep, debounced ...
	if(mcui_btn_state != mcui_btn_prev_state){
	    // ... and one or more buttons has changed state
	    changed_flags = mcui_btn_state ^ mcui_btn_prev_state;
	    mcui_btn_state = mcui_btn_prev_state;
	    // call changed handler
	    if(mcui_changed_handler){
		mcui_changed_handler(changed_flags, mcui_btn_state);
	    }
	    // check for clicks
	    mcui_detect_click(changed_flags, mcui_btn_state);
	}
    }
    // save current state for next time.
    mcui_btn_prev_state = buts;

    // handle rotary encoder turns
    if(mcui_re_turns){
	if(mcui_re_turned_handler)
	    mcui_re_turned_handler(mcui_re_turns);
	mcui_re_turns=0;
    }
}

//  -----------------------------------------------------------------------------
// Pin-change (PCINT) ISRs
ISR(PCINT0_vect)
{
    // Handle rotary encoder state machine (if necessary)
    MCUI_RE_PCINT0_vect();
}

ISR(PCINT1_vect)
{
    // Handle rotary encoder state machine (if necessary)
    MCUI_RE_PCINT1_vect();
}

ISR(PCINT2_vect)
{
    // Handle rotary encoder state machine (if necessary)
    MCUI_RE_PCINT2_vect();
}
//  -----------------------------------------------------------------------------
