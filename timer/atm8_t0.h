#ifndef ATM8_TIMER0_H
#define ATM8_TIMER0_H
// -----------------------------------------------------------------------------
// Copyright Stebbing Computing. 2013.
// $Id: atm8_t0.h 427 2016-04-07 04:21:09Z steves $
// Control of atmega8 timer 0. 8-bit timer with prescaller. Can interrupt on overflow.
// -----------------------------------------------------------------------------
#include <stdint.h>

// clock select bits - CS02-CS01. These are the 3 lsb in TCCR0 register
// No clk. Timer is stopped. As used by T0_INIT_CLK_SRC()
#define T0_CS_NO_CLK     0x0
// IO clk. No prescaling
#define T0_CS_CLK        0x1
// IO clk. prescaler div by 8
#define T0_CS_CLK_P8     0x02
// IO clk. prescaler div by 64
#define T0_CS_CLK_P64    0x03
// IO clk. prescaler div by 256
#define T0_CS_CLK_P256   0x04
// IO clk. prescaler div by 1024
#define T0_CS_CLK_P1024  0x05
// External clk. Falling edge.
#define T0_CS_CLK_E_FALL 0x06
// External cl, Rising edge.
#define T0_CS_CLK_E_RISE 0x06


// control structure definition
typedef struct {
    // value that TCCR0 reg will be set to when timer is started.
    // This sets the timer's clk source
    uint8_t tccr0;
}t0_ctl_t;

// global control structure
extern t0_ctl_t t0_ctl;



// Macros for timer control

//! enable the overflow interrupt - ensure prior that handler function has been specified
//! eg ISR(TIMER0_OVF_vect){}
#define T0_OI_E() TIMSK |= _BV(TOIE0)
//! disable the overflow interrupt
#define T0_OI_D() TIMSK &=~ _BV(TOIE0)

//! stop the timer by disabling the clock source
#define T0_STOP() TCCR0=T0_CS_NO_CLK
//! start the timer
#define T0_START()     TCCR0=t0_ctl.tccr0

// Initialise the clk source - use one of the T0_CS_XXX defines
#define T0_INIT_CLK_SRC(tccr0v) t0_ctl.tccr0=tccr0v


// Functions for timer control

#endif // ifnedf ATM8_TIMER0_H
