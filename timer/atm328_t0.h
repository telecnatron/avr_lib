#ifndef ATM328_T0_H
#define ATM328_T0_H
// -----------------------------------------------------------------------------
// Copyright Stebbing Computing. 2013.
// $Id: atm328_t0.h 427 2016-04-07 04:21:09Z steves $
// Control of atmega328 timer 0.
// -----------------------------------------------------------------------------

// Compare match behaviour:

// Output A
// Non-PWM modes:
// Normal port operation, OC0A disconnected
#define T0_OC0A_NONPWM_NORMAL() t0_ctl.tccr0a &=~ (_BV(COM0A1) | _BV(COM0A0))
// Toggle OC0A on compare match
#define T0_OC0A_NONPWM_TOGGLE() t0_ctl.tccr0a = (t0_ctl.tccr0a &~ _BV(COM0A1)) | _BV(COM0A0)
// Clear OC0A on compare match
#define T0_OC0A_NONPWM_CLEAR()  t0_ctl.tccr0a = (t0_ctl.tccr0a &~ _BV(COM0A0)) | _BV(COM0A1)
// Set OC0A on compare match
#define T0_OC0A_NONPWM_SET()    t0_ctl.tccr0a |=  (_BV(COM0A1) | _BV(COM0A0))

// Fast PWM Mode
// Normal port operation, OC0A disconnected
#define T0_OC0A_PWMF_NORMAL() t0_ctl.tccr0a &=~ (_BV(COM0A1) | _BV(COM0A0))
// In conjunction with WGM02: toggle on compare match
#define T0_OC0A_PWMF_TOGGLE() t0_ctl.tccr0a = (t0_ctl.tccr0a &~ _BV(COM0A1)) | _BV(COM0A0)
// Set when down-counting, clear when up-counting
#define T0_OC0A_PWMF_CLEAR()  t0_ctl.tccr0a = (t0_ctl.tccr0a &~ _BV(COM0A0)) | _BV(COM0A1)
// Set when upcounting, clear when downcounting
#define T0_OC0A_PWMF_SET()    t0_ctl.tccr0a |=  (_BV(COM0A1) | _BV(COM0A0))

// Phase Correct PWM Mode
// Set when down-counting, clear when up-counting
#define T0_OC0A_PWMPC_CLEAR() T0_OC0A_PWMF_CLEAR()  

// Output B
// Non-PWM modes:
// Normal port operation, OC0B disconnected
#define T0_OC0B_NONPWM_NORMAL() t0_ctl.tccr0a &=~ (_BV(COM0B1) | _BV(COM0B0))
// Toggle OC0B on compare match
#define T0_OC0B_NONPWM_TOGGLE() t0_ctl.tccr0a = (t0_ctl.tccr0a &~ _BV(COM0B1)) | _BV(COM0B0)
// Clear OC0B on compare match
#define T0_OC0B_NONPWM_CLEAR()  t0_ctl.tccr0a = (t0_ctl.tccr0a &~ _BV(COM0B0)) | _BV(COM0B1)
// Set OC0A on compare match
#define T0_OC0B_NONPWM_SET()    t0_ctl.tccr0a |=  (_BV(COM0B1) | _BV(COM0B0))

// Fast PWM Mode
// Normal port operation, OC0B disconnected
#define T0_OC0B_PWMF_NORMAL() t0_ctl.tccr0a &=~ (_BV(COM0B1) | _BV(COM0B0))
// reserved
//#define T0_OC0B_PWMF_RESERVED() t0_ctl.tccr0a = (t0_ctl.tccr0a &~ _BV(COM0B1)) | _BV(COM0B0)
// Set when down-counting, clear when up-counting
#define T0_OC0B_PWMF_CLEAR()  t0_ctl.tccr0a = (t0_ctl.tccr0a &~ _BV(COM0B0)) | _BV(COM0B1)
// Set when upcounting, clear when downcounting
#define T0_OC0B_PWMF_SET()    t0_ctl.tccr0a |=  (_BV(COM0B1) | _BV(COM0B0))

// Phase Correct PWM Mode
// Set when down-counting, clear when up-counting
#define T0_OC0B_PWMPC_CLEAR()  T0_OC0B_PWMF_CLEAR() 

// -----------------------------------------------------------------------------
// Waveform generation
// normal mode. OCRx updated immediately, TOV set at top, top=0xff
#define T0_WGM_NORMAL() t0_ctl.tccr0a &=~ (_BV(WGM01) | _BV(WGM00) ); t0_ctl.tccr0b &=~ (_BV(WGM02))
// PWM phase correct. OCRx set at top, TOV set at bottom, top=0xff
#define T0_WGM_PWMPC() t0_ctl.tccr0a = (( t0_ctl.tccr0a &= _BV(WGM01)) | _BV(WGM00)  ); t0_ctl.tccr0b &=~ (_BV(WGM02))
// CTC mode. OCR set immediately, TOV set at MAX, top = OCRA
#define T0_WGM_CTC()  (t0_ctl.tccr0a = (( t0_ctl.tccr0a &= _BV(WGM00)) | _BV(WGM01)  ); t0_ctl.tccr0b &=~ (_BV(WGM02))
// PWM fast
#define T0_WGM_PWMF() t0_ctl.tccr0a |= ( _BV(WGM00) | _BV(WGM01)  ); t0_ctl.tccr0b &=~ (_BV(WGM02))

// XXX more here for when using OCR0A for top
// -----------------------------------------------------------------------------
// Clock select
// Clock select bits. Use this to select prescaller value. Clock is enable when these bits are set.
// Use T0_STOP() to clear these bits and hence stop the clock.
// Note that timer0 and timer1 share the same prescaller

#define T0_CS_PRE_1()    t0_ctl.tccr0b = ( t0_ctl.tccr0b &~ ( _BV(CS02) | _BV(CS01) ) ) | _BV(CS00)
#define T0_CS_PRE_8()    t0_ctl.tccr0b = ( t0_ctl.tccr0b &~ ( _BV(CS02) | _BV(CS00) ) ) | _BV(CS01)
#define T0_CS_PRE_64()   t0_ctl.tccr0b = ( t0_ctl.tccr0b &~ ( _BV(CS02)  ) ) | _BV(CS01) | _BV(CS00)
#define T0_CS_PRE_256()  t0_ctl.tccr0b = ( t0_ctl.tccr0b &~ ( _BV(CS01) | _BV(CS00) ) )  | _BV(CS02)
#define T0_CS_PRE_1024() t0_ctl.tccr0b =  ( t0_ctl.tccr0b &~ ( _BV(CS01) ) )  | _BV(CS02) | _BV(CS00)
// External clk on T0 pin. Clock on falling edge
#define T0_CS_PRE_FALL() t0_ctl.tccr0b =  ( t0_ctl.tccr0b &~ ( _BV(CS00) ) )  | _BV(CS02) | _BV(CS01)
// External clk on T0 pin. Clock on rise
#define T0_CS_PRE_RISE() t0_ctl.tccr0b =  t0_ctl.tccr0b | _BV(CS02) | _BV(CS01) | _BV(CS00)

// -----------------------------------------------------------------------------
// enable output-compare OCOA as output,
// port d6 for atmega88/168/328, pin "4" on Ardunio Pro Mini, pin 10 on DIL, 
#define T0_OE()    DDRD |= _BV(PIN6)

// Set the OCR value - compare match is triggered at this value
#define T0_OCR0A(value) OCR0A = value
#define T0_OCR0B(value) OCR0B = value

//! stop the timer by disabling the clock source
#define T0_STOP() TCCR0A &=~ ( _BV(CS02) | _BV(CS01) | _BV(CS00) )
//! start the timer
#define T0_START()     TCCR0A=t0_ctl.tccr0a; TCCR0B=t0_ctl.tccr0b

//! enable the overflow interrupt - ensure prior that handler function has been specified
//! eg ISR(TIMER0_OVF_vect){}
#define T0_OI_E() TIMSK0 |= _BV(TOIE0)
//! disable the overflow interrupt
#define T0_OI_D() TIMSK0 &=~ _BV(TOIE0)

// Output compare interrupt
//! enable the output compare match interrupt on A 
#define T0_CMA_E() TIMSK0 |= _BV(OCIE0A)
//! disable the output compare match interrupt on A
#define T0_CMA_D() TIMSK0 &=~ _BV(OCIE0A)

//! enable the output compare match interrupt on B
#define T0_CMB_E() TIMESK |= _BV(OCIE0B)
//! disable the output compare match interrupt on B
#define T0_CMB_D() TIMSK0 &=~ _BV(OCIE0B)

// -----------------------------------------------------------------------------


// control structure definition
typedef struct {
    // value that TCCR0x regs will be set to when timer is started.
    uint8_t tccr0a;
    uint8_t tccr0b;
}t0_ctl_t;

// global control structure
static volatile t0_ctl_t t0_ctl;


#endif
