#ifndef ATM328_T2_H
#define ATM328_T2_H
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
#include <stdint.h>
#include <avr/io.h>

// Compare match behaviour:

// Output A
// Non-PWM modes:
// Normal port operation, OC2A disconnected
#define T2_OC2A_NONPWM_NORMAL() t2_ctl.tccr2a &=~ (_BV(COM2A1) | _BV(COM2A0))
// Toggle OC2A on compare match
#define T2_OC2A_NONPWM_TOGGLE() t2_ctl.tccr2a = (t2_ctl.tccr2a &~ _BV(COM2A1)) | _BV(COM2A0)
// Clear OC2A on compare match
#define T2_OC2A_NONPWM_CLEAR()  t2_ctl.tccr2a = (t2_ctl.tccr2a &~ _BV(COM2A0)) | _BV(COM2A1)
// Set OC2A on compare match
#define T2_OC2A_NONPWM_SET()    t2_ctl.tccr2a |=  (_BV(COM2A1) | _BV(COM2A0))

// Fast PWM Mode
// Normal port operation, OC2A disconnected
#define T2_OC2A_PWMF_NORMAL() t2_ctl.tccr2a &=~ (_BV(COM2A1) | _BV(COM2A0))
// In conjunction with WGM02: toggle on compare match
#define T2_OC2A_PWMF_TOGGLE() t2_ctl.tccr2a = (t2_ctl.tccr2a &~ _BV(COM2A1)) | _BV(COM2A0)
// Set when down-counting, clear when up-counting
#define T2_OC2A_PWMF_CLEAR()  t2_ctl.tccr2a = (t2_ctl.tccr2a &~ _BV(COM2A0)) | _BV(COM2A1)
// Set when upcounting, clear when downcounting
#define T2_OC2A_PWMF_SET()    t2_ctl.tccr2a |=  (_BV(COM2A1) | _BV(COM2A0))

// Phase Correct PWM Mode
// XXX goes here

// Output B
// Non-PWM modes:
// Normal port operation, OC2B disconnected
#define T2_OC2B_NONPWM_NORMAL() t2_ctl.tccr2a &=~ (_BV(COM2B1) | _BV(COM2B0))
// Toggle OC2B on compare match
#define T2_OC2B_NONPWM_TOGGLE() t2_ctl.tccr2a = (t2_ctl.tccr2a &~ _BV(COM2B1)) | _BV(COM2B0)
// Clear OC2B on compare match
#define T2_OC2B_NONPWM_CLEAR()  t2_ctl.tccr2a = (t2_ctl.tccr2a &~ _BV(COM2B0)) | _BV(COM2B1)
// Set OC2B on compare match
#define T2_OC2B_NONPWM_SET()    t2_ctl.tccr2a |=  (_BV(COM2B1) | _BV(COM2B0))

// Fast PWM Mode
// Normal port operation, OC2B disconnected
#define T2_OC2B_PWMF_NORMAL() t2_ctl.tccr2a&=~ (_BV(COM2B1) | _BV(COM2B0))
// reserved
//#define T2_OC2B_PWMF_RESERVED() t2_ctl.tccr2a= (t2_ctl.tccr2a&~ _BV(COM2B1)) | _BV(COM2B0)
// Set when down-counting, clear when up-counting
#define T2_OC2B_PWMF_CLEAR()  t2_ctl.tccr2a = (t2_ctl.tccr2a &~ _BV(COM2B0)) | _BV(COM2B1)
// Set when upcounting, clear when downcounting
#define T2_OC2B_PWMF_SET()    t2_ctl.tccr2a |=  (_BV(COM2B1) | _BV(COM2B0))

// Phase Correct PWM Mode
// XXX goes here

// -----------------------------------------------------------------------------
// Waveform generation
// normal mode. OCRx updated immediately, TOV set at top, top=0xff
#define T2_WGM_NORMAL() t2_ctl.tccr2a &=~ (_BV(WGM21) | _BV(WGM20) ); t2_ctl.tccr2b &=~ (_BV(WGM22))
// PWM phase correct. OCRx set at top, TOV set at bottom, top=0xff
#define T2_WGM_PWMPC() t2_ctl.tccr2a = ( t2_ctl.tccr2a &= _BV(WGM21)) | _BV(WGM20)  ); t2_ctl.tccr2b &=~ (_BV(WGM22))
// CTC mode. OCR set immediately, TOV set at MAX, top = OCRA
#define T2_WGM_CTC()  (t2_ctl.tccr2a = ( t2_ctl.tccr2a &= _BV(WGM20)) | _BV(WGM21)  ); t2_ctl.tccr2b &=~ (_BV(WGM22))
// PWM fast
#define T2_WGM_PWMF() t2_ctl.tccr2a |= ( _BV(WGM20) | _BV(WGM21)  ); t2_ctl.tccr2b &=~ (_BV(WGM22))

// XXX more here for when using OCR0A for top
// -----------------------------------------------------------------------------
// Clock select
// Clock select bits. Use this to select prescaller value. Clock is enable when these bits are set.
// Use T2_STOP() to clear these bits and hence stop the clock.
#define T2_CS_PRE_1()    t2_ctl.tccr2b = ( t2_ctl.tccr2b &~ ( _BV(CS22)| _BV(CS21))) | _BV(CS20)
#define T2_CS_PRE_8()    t2_ctl.tccr2b = ( t2_ctl.tccr2b &~ ( _BV(CS22)| _BV(CS20))) | _BV(CS21)
#define T2_CS_PRE_32()   t2_ctl.tccr2b = ( t2_ctl.tccr2b &~  _BV(CS22))  | _BV(CS20)   | _BV(CS21)
#define T2_CS_PRE_64()   t2_ctl.tccr2b = ( t2_ctl.tccr2b &~  (_BV(CS21) | _BV(CS20))) | _BV(CS22)
#define T2_CS_PRE_128()  t2_ctl.tccr2b = ( t2_ctl.tccr2b &~  _BV(CS21)) | _BV(CS20)   | _BV(CS22)
#define T2_CS_PRE_256()  t2_ctl.tccr2b = ( t2_ctl.tccr2b &~  _BV(CS20))  | _BV(CS21)   | _BV(CS22)
#define T2_CS_PRE_1024() t2_ctl.tccr2b = ( t2_ctl.tccr2b) | _BV(CS21)    | _BV(CS22)   | _BV(CS20)

// External clk on T0 pin. Clock on falling edge
#define T2_CS_PRE_FALL() t2_ctl.tccr2b =  ( t2_ctl.tccr2b &~ ( _BV(CS20) ) )  | _BV(CS22) | _BV(CS21)
// External clk on T0 pin. Clock on rise
#define T2_CS_PRE_RISE() t2_ctl.tccr2b =  t2_ctl.tccr2b | _BV(CS22) | _BV(CS21) | _BV(CS20)

// -----------------------------------------------------------------------------
// enable OC2A as output
#define T2_OE()    DDRB |= _BV(PIN3)

// Set the OCR value - compare match is triggered at this value
#define T2_OCR2A(value) OCR2A = value
#define T2_OCR2B(value) OCR2B = value

//! stop the timer by disabling the clock source
#define T2_STOP() TCCR2B &=~ ( _BV(CS22) | _BV(CS21) | _BV(CS20) )
//! start the timer
#define T2_START()     TCCR2A=t2_ctl.tccr2a; TCCR2B=t2_ctl.tccr2b

//! enable the overflow interrupt - ensure prior that handler function has been specified
//! eg ISR(TIMER0_OVF_vect){}
#define T2_OI_E() TIMSK2 |= _BV(TOIE2)
//! disable the overflow interrupt
#define T2_OI_D() TIMSK2 &=~ _BV(TOIE2)

// Output compare interrupt
//! enable the output compare match interrupt on A 
#define T2_CMA_E() TIMSK2 |= _BV(OCIE2A)
//! disable the output compare match interrupt on A
#define T2_CMA_D() TIMSK2 &=~ _BV(OCIE2A)

//! enable the output compare match interrupt on B
#define T2_CMB_E() TIMSK2 |= _BV(OCIE2B)
//! disable the output compare match interrupt on B
#define T2_CMB_D() TIMSK2 &=~ _BV(OCIE2B)

// -----------------------------------------------------------------------------
//! Read the value of the timer count register
#define T2_READ() TCNT2
//! Set the vlaue of the timer count register
#define T2_WRITE(byte) TCNT2=byte

// control structure definition
typedef struct {
    // value that TCCR2x regs will be set to when timer is started.
    uint8_t tccr2a;
    uint8_t tccr2b;
}t2_ctl_t;

// global control structure
volatile t2_ctl_t t2_ctl;


#endif
