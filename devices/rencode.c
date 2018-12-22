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
#include "rencode.h"
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "../util/io.h"

// Globals
// pointer to function that gets called when ISR detectes a click.
// dir indicates direction: 0 or 1
// Note: this is called from withing the ISR
static void (* re_handler)(uint8_t dir);

//! init rotary encoder pins as inputs, PUs enabled.
void re_init(void (* re_handler_fn)(uint8_t dir))
{
    // config as input
    PIN_LO(RE_CTL,RE_PIN0);
    PIN_LO(RE_CTL,RE_PIN1);
    // set output high to enable internal pullup
    PIN_HI(RE_W,RE_PIN0);
    PIN_HI(RE_W,RE_PIN1);

    // set the mask for pin change mask register
    PIN_HI(RE_PCMSK_REG, RE_PIN0);
    PIN_HI(RE_PCMSK_REG, RE_PIN1);
    // enable pin change interrupt
    PIN_HI(PCICR,RE_PCINT);

    re_handler=re_handler_fn;
}

//! Reads the rotary encoder's pins, returns (with inverted levels, ie asserted corresponds to logical low)
//! a uint8_t with bit0 set to 1 if RE_PIN0 is asserted, 0 otherwise,
//! and bit1 set to 1 if RE_PIN1 is asserted, 0 otherwise.
//! eg 0x3 indicates that both pins are asserted.
static inline uint8_t re_read()
{
    if(RE_R & _BV(RE_PIN0)){
	// pin 0 is high (ie not asserted)
	if(RE_R & _BV(RE_PIN1)){
	    // pin 1 is high
	    return 0x0;
	}else{
	    return 0x2;
	}
    }else{
	// pin 0 is low
	if(RE_R & _BV(RE_PIN1)){
	    // pin 1 is high
	    return 0x1;
	}else{
	    // pin 1 is low
	    return 0x3;
	}
    }
}


//! Rotary encoder state machine. Should be called from ISR(RE_ISR_NAME) routine.
//! see https://www.circuitsathome.com/mcu/rotary-encoder-interrupt-service-routine-for-avr-micros
inline void re_isr_handler()
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
    re_prev |= re_read();
    // lookup up increment value in state table
    re_value += pgm_read_byte(&(re_states[( re_prev & 0x0f )]));

    if(re_value > 3){
        re_handler(1);
	re_value=0;
    }else  if(re_value < -3){
        re_handler(0);
	re_value=0;
    }
}
