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
 * @brief  
 * 
 */
#include "mcui.defs"
#include <stdint.h>
#include "./lib/util/io.h"

// push button and re bit values, bit being high indicates that corresponding button is asserted
#define MCUI_PB0  0
#define MCUI_PB1  1
#define MCUI_PB2  2
#define MCUI_PB3  3
#define MCUI_PB4  4
#define MCUI_PB5  5

#define MCUI_ASSERTED_PB0(byte) (byte & _BV(MCUI_PB0))
#define MCUI_ASSERTED_PB1(byte) (byte & _BV(MCUI_PB1))
#define MCUI_ASSERTED_PB2(byte) (byte & _BV(MCUI_PB2))
#define MCUI_ASSERTED_PB3(byte) (byte & _BV(MCUI_PB3))
#define MCUI_ASSERTED_PB4(byte) (byte & _BV(MCUI_PB4))
#define MCUI_ASSERTED_PB5(byte) (byte & _BV(MCUI_PB5))

// maximum number of ticks between button being down-and-then-up for it to be 
// considered as a click.
#define MCUI_CLICK_TICKS 750

//!
void mcui_init(void (*changed_fn), void (*click_fn), void(*re_fn));

//! Returns current tick count
uint16_t mcui_get_ticks();

//!
void mcui_tick();

//!
uint8_t mcui_read_btns();

#endif /* _MCUI_H */

