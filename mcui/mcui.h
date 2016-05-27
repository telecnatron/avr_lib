#ifndef _MCUI_H
#define _MCUI_H 1
// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2016. http://telecnatron.com/
// -----------------------------------------------------------------------------
/**
 * @file   mcui.h
 * @author steves
 * @date   2016/05/16 07:45:23
 * 
 * @brief  Microcontroller Common User Interface for ATMEGAx8 CPUs.
 * HW interface consists of a rotary encoder, up to 8 push buttons, and up to 8 leds.
 * 
 * HW pin assignments are read from mcui.defs file which is created with the help of
 * a utility program: mcui.py
 * 
 * PBs and RE switches are polled, debounced and decoded by means of periodic calls to 
 * mcui_tick() function. User is notified of PB changed and clicked events, and RE events
 * by means of callback functions.
 * 
 * Note that pin-change ISRs are defined in mcui.c
 * 
 * See http://telecnatron.com/articles/mcui for further details.
 */

#include "mcui.defs"
#include <stdint.h>
#include "./lib/util/io.h"

//! push button and re bit values, bit being high indicates that corresponding button is asserted
#define MCUI_PB0  0
#define MCUI_PB1  1
#define MCUI_PB2  2
#define MCUI_PB3  3
#define MCUI_PB4  4
#define MCUI_PB5  5

//! Given the passed bit fields, determine if correspond is asserted or not.
#define MCUI_ASSERTED_PB0(byte) (byte & _BV(MCUI_PB0))
#define MCUI_ASSERTED_PB1(byte) (byte & _BV(MCUI_PB1))
#define MCUI_ASSERTED_PB2(byte) (byte & _BV(MCUI_PB2))
#define MCUI_ASSERTED_PB3(byte) (byte & _BV(MCUI_PB3))
#define MCUI_ASSERTED_PB4(byte) (byte & _BV(MCUI_PB4))
#define MCUI_ASSERTED_PB5(byte) (byte & _BV(MCUI_PB5))

//! Maximum number of ticks between button being down-and-then-up for it to be considered as a click.
//! With a 1ms tick, 750 seems about right and corresponds or 0.75 seconds. To use a different value, 
//! define this prior to here.
#ifndef MCUI_CLICK_TICKS
#define MCUI_CLICK_TICKS 750
#endif

/** 
 * Initialise the system and install user-event handlers. (These may be NULL) 
 * 
 * @param changed_fn Function that gets called when a button has changed, parameters being bit-mask corresponding
 * to buttons that have changed state since last call, and bit-mask containing actual state of the buttons.
 * @param click_fn Called when a button click has been detected, passed the index of the button that was clicked.
 * @param re_fn Called when rotary encoder movement has been detected since last call, parameter contains number 
 * of turns that were detected (usually 1) and sign indicates direction.
 */
void mcui_init(void (*changed_fn)(uint8_t pbs_changed, uint8_t pbs_state),
	       void (*click_fn)(uint8_t btn_index), 
	       void(*re_fn)(int8_t dir));

//! Returns current tick count
uint16_t mcui_get_ticks();

/** 
 * Poll PBs and RE and call event handlers as necessary.
 * PB debouncing is done by requiring that the PB states remain unchanged over two subsequent calls. 
 * With tick interval of 1ms, this seems to work well.
 */
void mcui_tick();

/** 
 * Return bitmask being current state of PBs
 * @return Bitmask corresponding to current state of PBs
 */
uint8_t mcui_read_pbs();

#endif /* _MCUI_H */


