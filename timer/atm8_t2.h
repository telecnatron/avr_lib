#ifndef ATM8_T2_H
#define ATM8_T2_H
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

// Waveform generation settings
#define T2_WGM_NORMAL() t2_ctl.tccr2 &=~ (_BV(WGM21) | _BV(WGM20))
// Clear timer on compare match.
#define T2_WGM_CTC()    t2_ctl.tccr2 = (t2_ctl.tccr2 &~ _BV(WGM20)) | _BV(WGM21)
// Phase correct PWM.
#define T2_WGM_PWMPC() t2_ctl.tccr2 = (t2_ctl.tccr2 &~ _BV(WGM21)) | _BV(WGM20)
// Fast PWM.
#define T2_WGM_PWMF()  t2_ctl.tccr2 |=  (_BV(WGM20) | _BV(WGM21))

// Clock select bits. Use this to select prescaller value. Clock is enable when these bits are set.
// Use T2_STOP() to clear these bits and hence stop the clock.
#define T2_CS_PRE_1()    t2_ctl.tccr2 = ( t2_ctl.tccr2 &~ ( _BV(CS22) | _BV(CS21) ) ) | _BV(CS20)
#define T2_CS_PRE_8()    t2_ctl.tccr2 = ( t2_ctl.tccr2 &~ ( _BV(CS22) | _BV(CS20) ) ) | _BV(CS21)
#define T2_CS_PRE_32()   t2_ctl.tccr2 = ( t2_ctl.tccr2 &~  _BV(CS22)) | _BV(CS20) | _BV(CS21)
#define T2_CS_PRE_64()   t2_ctl.tccr2 = ( t2_ctl.tccr2 &~ ( _BV(CS21) | _BV(CS20) ) ) | _BV(CS22)
#define T2_CS_PRE_128()  t2_ctl.tccr2 = ( t2_ctl.tccr2 &~  _BV(CS21)) | _BV(CS22) | _BV(CS20)
#define T2_CS_PRE_256()  t2_ctl.tccr2 = ( t2_ctl.tccr2 &~  _BV(CS20)) | _BV(CS22) | _BV(CS21)
#define T2_CS_PRE_1024() t2_ctl.tccr2 =  t2_ctl.tccr2 | _BV(CS22) | _BV(CS21) | _BV(CS20)

// Output compare pin setting OC2 . Note that, if used, it must be set as an output.
// OC2 is chip pin 17, PB3

// set OC2 pin as output. PB3, pin 17
#define T2_OC2_INIT_OUTPUT() DDRB |= _BV(PIN3);

// Compare match behaviour:

// Non-PWM modes:
// Normal port operation, OC2 disconnected
#define T2_OC2_NONPWM_NORMAL() t2_ctl.tccr2 &=~ (_BV(COM21) | _BV(COM20))
// Toggle OC2 on compare match
#define T2_OC2_NONPWM_TOGGLE() t2_ctl.tccr2 = (t2_ctl.tccr2 &~ _BV(COM21)) | _BV(COM20)
// Clear OC2 on compare match
#define T2_OC2_NONPWM_CLEAR()  t2_ctl.tccr2 = (t2_ctl.tccr2 &~ _BV(COM20)) | _BV(COM21)
// Set OC2 on compare match
#define T2_OC2_NONPWM_SET()    t2_ctl.tccr2 |=  (_BV(COM21) | _BV(COM20))

// Fast PWM mode
// Normal port operation, OC2 disconnected
#define T2_OC2_PWMF_NORMAL() t2_ctl.tccr2 &=~ (_BV(COM21) | _BV(COM20))
// Clear OC2 when up counting, set when down counting
#define T2_OC2_PWMF_CLEAR()  t2_ctl.tccr2 = (t2_ctl.tccr2 &~ _BV(COM20)) | _BV(COM21)
// Set OC2 on compare match, clear OC2 at bottom (inverting mode)
#define T2_OC2_PWMF_SET()    t2_ctl.tccr2 |=  (_BV(COM21) | _BV(COM20))

// Phase correct PWM mode
// Normal port operation, OC2 disconnected
#define T2_OC2_PWMPC_NORMAL() t2_ctl.tccr2 &=~ (_BV(COM21) | _BV(COM20))
// Clear OC2 on compare match when up-counting. Set OC2 on cm when downcounting
#define T2_OC2_PWMPC_CLEAR()  t2_ctl.tccr2 = (t2_ctl.tccr2 &~ _BV(COM20)) | _BV(COM21)
// Set OC2 on compare match when up-counting. Clear OC2 on cm when down-counting.
#define T2_OC2_PWMPC_SET()    t2_ctl.tccr2 |=  (_BV(COM21) | _BV(COM20))


// Set the OCR2 value - compare match is triggered at this value
#define T2_OCR(value) OCR2 = value

//! stop the timer by disabling the clock source
#define T2_STOP() TCCR2 &=~ ( _BV(CS22) | _BV(CS21) | _BV(CS20) )
//! start the timer
#define T2_START()     TCCR2=t2_ctl.tccr2

//! enable the overflow interrupt - ensure prior that handler function has been specified
//! eg ISR(TIMER2_OVF_vect){}
#define T2_OI_E() TIMSK |= _BV(TOIE2)
//! disable the overflow interrupt
#define T2_OI_D() TIMSK &=~ _BV(TOIE2)

// Output compare interrupt
//! enable the output compare match interrupt
#define T2_CM_E() TIMESK |= _BV(OCIE2)
//! disable the output compare match interrupt
#define T2_CM_D() TIMSK &=~ _BV(OCIE2)



// control structure definition
typedef struct {
    // value that TCCR2 reg will be set to when timer is started.
    // This sets the timer's clk source
    uint8_t tccr2;
}t2_ctl_t;

// global control structure
extern t2_ctl_t t2_ctl;


#endif
