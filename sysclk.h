#ifndef _SYSCLK_H
#define _SYSCLK_H 1
// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2016. http://telecnatron.com/
// -----------------------------------------------------------------------------
/**
 * @file   sysclk.h
 * @author steves
 * @date   2016/05/16 06:32:46
 * 
 * @brief  Timer that counts ticks and 'seconds'. Default setting is to use T2:
 * FCPU 16MHz, prescaller 1024, terminal count 15, for timer period of 1.024ms
 * which is counted as 1 'second'. 
 *
 * Uses TIMER2_COMPA_vect, ISR is defined in sysclk.c
 * 
 * To changed these settings edit the SYSCLK_xxx defines/macros.
 * 
 */
#include "./timer.h"
#include "config.h"
#include <stdint.h>

//! normal mode, output pin disabled, set CTC mode - clear timer on compare match
#define SYSCLK_INIT()        T2_OC2A_NONPWM_NORMAL(); T2_WGM_CTC()
#define SYSCLK_INT_ENABLE()  T2_CMA_E();
#define SYSCLK_INT_DISABLE() T2_CMA_D();
#define SYSCLK_START()       T2_START();
#define SYSCLK_STOP()        T2_STOP();
#define SYSCLK_ISR_NAME      TIMER2_COMPA_vect

//! With FCPU = 16MHz, prescaler 1024 and terminal count 15 gives 
//! tick (compare match interrupt) period of 1.024ms, ~976Hz
#define SYSCLK_INIT_PRE()  T2_CS_PRE_1024()
#define SYSCLK_SET_TC()    T2_OCR2A(15)
//! This many ticks per 'second'
#define SYSCLK_TICK_FREQ   976

//! Initialise and start the clock.
void sysclk_init();

//! Stop the clock. Disable interrupt.
void sysclk_stop();

//! Start the clock. Enable interrupt.
void sysclk_start();

//! Resets tick and seconds counter to 0, clears tick flags
void sysclk_reset();

//! Return the current tick count
uint16_t sysclk_get_ticks();

//! Return true if a tick has occured since prior call, false otherwise.
uint8_t sysclk_has_ticked();

//! Return current seconds count
uint16_t sysclk_get_seconds();

//! Return true if a seconds-tick has occured since prior call, false otherwise.
uint8_t sysclk_have_seconds_ticked();

//! Reset the seconds count to 0
void sysclk_reset_seconds();


#endif /* _SYSCLK_H */

