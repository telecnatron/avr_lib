#ifndef ATT13_T0_H
#define ATT13_T0_H
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
// compare output modes

// Fast PWM Mode
// Normal port operation, OC0A disconnected
#define T0_OCA_PWMF_NORMAL() t0_ctl.tccr0a &=~ (_BV(COM0A1) | _BV(COM0A0))
// In conjunction with WGM02: toggle on compare match
#define T0_OCA_PWMF_TOGGLE() t0_ctl.tccr0a = (t0_ctl.tccr0a &~ _BV(COM0A1)) | _BV(COM0A0)
// Clear OC0A on compare match, set OC0A at top
#define T0_OCA_PWMF_CLEAR()  t0_ctl.tccr0a = (t0_ctl.tccr0a &~ _BV(COM0A0)) | _BV(COM0A1)
// Set OC0A on compare match, clear at top
#define T0_OCA_PWMF_SET()    t0_ctl.tccr0a |=  (_BV(COM0A1) | _BV(COM0A0))

// Normal port operation, OC0B disconnected
#define T0_OCB_PWMF_NORMAL() t0_ctl.tccr0a &=~ (_BV(COM0B1) | _BV(COM0B0))
//XXX not available for OCB: In conjunction with WGM02: toggle on compare match
//#define T0_OCB_PWMF_TOGGLE() t0_ctl.tccr0b = (t0_ctl.tccr0b &~ _BV(COM0B1)) | _BV(COM0B0)
// Clear OC0B on compare match, set OC0B at top
#define T0_OCB_PWMF_CLEAR()  t0_ctl.tccr0a = (t0_ctl.tccr0a &~ _BV(COM0B0)) | _BV(COM0B1)
// Set OC0B on compare match, clear at top
#define T0_OCB_PWMF_SET()    t0_ctl.tccr0a |=  (_BV(COM0B1) | _BV(COM0B0))

// -----------------------------------------------------------------------------
// Waveform generation
// normal mode. OCR updated immediately, TOV set at top, top=0xff
#define T0_WGM_NORMAL() t0_ctl.tccr0a &=~ (_BV(WGM01) | _BV(WGM00) ); t0_ctl.tccr0b &=~ (_BV(WGM02))
// PWM phase correct. OCR set at top, TOV set at bottom, top=0xff
#define T0_WGM_PWMPC() t0_ctl.tccr0a = ( t0_ctl.tccr0a &= _BV(WGM01)) | _BV(WGM00)  ); t0_ctl.tccr0b &=~ (_BV(WGM02))
// CTC mode. OCR set immediately, TOV set at MAX, top = OCRA
#define T0_WGM_CTC()  t0_ctl.tccr0a = ( t0_ctl.tccr0a &= _BV(WGM00)) | _BV(WGM01)  ); t0_ctl.tccr0b &=~ (_BV(WGM02))
// PWM fast
#define T0_WGM_PWMF() t0_ctl.tccr0a |= ( _BV(WGM00) | _BV(WGM01)  ); t0_ctl.tccr0b &=~ (_BV(WGM02))

// XXX more here for when using OCR0A for top

// -----------------------------------------------------------------------------
// Clock select

// Clock select bits. Use this to select prescaller value. Clock is enable when these bits are set.
// Use T2_STOP() to clear these bits and hence stop the clock.
#define T0_CS_PRE_1()    t0_ctl.tccr0b = ( t0_ctl.tccr0b &~ ( _BV(CS02) | _BV(CS01) ) ) | _BV(CS00)
#define T0_CS_PRE_8()    t0_ctl.tccr0b = ( t0_ctl.tccr0b &~ ( _BV(CS02) | _BV(CS00) ) ) | _BV(CS01)
#define T0_CS_PRE_64()   t0_ctl.tccr0b = ( t0_ctl.tccr0b &~ ( _BV(CS02)  ) ) | _BV(CS01) | _BV(CS00)
#define T0_CS_PRE_256()  t0_ctl.tccr0b = ( t0_ctl.tccr0b &~ ( _BV(CS01) | _BV(CS00) ) )  | _BV(CS02)
// External clk on T0 pin. Clock on rise
#define T0_CS_PRE_1024() t0_ctl.tccr0b =  ( t0_ctl.tccr0b &~ ( BV(CS01) ) )  | _BV(CS02) | _BV(CS00)
// External clk on T0 pin. Clock on falling edge
#define T0_CS_PRE_FALL() t0_ctl.tccr0b =  ( t0_ctl.tccr0b &~ ( BV(CS00) ) )  | _BV(CS02) | _BV(CS01)
// External clk on T0 pin. Clock on rise
#define T0_CS_PRE_RISE() t0_ctl.tccr0b =  t0_ctl.tccr0b | _BV(CS02) | _BV(CS01) | _BV(CS00)

// -----------------------------------------------------------------------------
// output compare settings

// enable OCOA as output, chip pin 5 for dip, port b0
#define T0_OAE()    DDRB |= _BV(PIN0)
// enable OCOB as output, chip pin 6 for dip, port b1
#define T0_OBE()    DDRB |= _BV(PIN1)

// Set the OCR value - compare match is triggered at this value
#define T0_OCR0A(value) OCR0A = value
#define T0_OCR0B(value) OCR0B = value

// stop/start timer

//! stop the timer by disabling the clock source
#define T0_STOP() TCCR0A &=~ ( _BV(CS02) | _BV(CS01) | _BV(CS00) )
//! start the timer
#define T0_START()     TCCR0A=t0_ctl.tccr0a; TCCR0B=t0_ctl.tccr0b

// interrupts

//! enable the overflow interrupt - ensure prior that handler function has been specified
//! eg ISR(TIM0_OVF_vect){}
#define T0_OI_E() TIMSK0 |= _BV(TOIE0)
//! disable the overflow interrupt
#define T0_OI_D() TIMSK0 &=~ _BV(TOIE0)

//! enable the output compare match interrupt on A 
//! eg ISR(TIM0_COMPA_vect){}
#define T0_CMA_E() TIMSK0 |= _BV(OCIE0A)
//! disable the output compare match interrupt on A
#define T0_CMA_D() TIMSK0 &=~ _BV(OCIE0A)

//! enable the output compare match interrupt on B
//! eg ISR(TIM0_COMPB_vect){}
#define T0_CMB_E() TIMSK0 |= _BV(OCIE0B)
//! disable the output compare match interrupt on B
#define T0_CMB_D() TIMSK0 &=~ _BV(OCIE0B)


// -----------------------------------------------------------------------------
// control structure definition
typedef struct {
    // values that TCCR0x regs will be set to when timer is started.
    uint8_t tccr0a;
    uint8_t tccr0b;
}t0_ctl_t;

// global control structure
volatile static t0_ctl_t t0_ctl;

#endif
