// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2016. http://telecnatron.com/
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// -----------------------------------------------------------------------------
#include "sysclk.h"
#include <avr/interrupt.h>

// tick count
uint16_t sysclk_ticks;
// seconds count
uint16_t sysclk_seconds;
// flag to indicated that clk has ticked since last call to sysclk_has_ticked()
uint8_t sysclk_ticked;
// flag to indicated that seconds haveticked since last call to sysclk_have_seconds_ticked()
uint8_t sysclk_seconds_ticked;


void sysclk_init()
{
    SYSCLK_INIT();
    SYSCLK_INIT_PRE();
    SYSCLK_SET_TC();
    sysclk_start();
}

inline void sysclk_stop()
{
    // disable clk source
    SYSCLK_STOP();
    // disable interrupt
    SYSCLK_INT_DISABLE();
}

inline void sysclk_start()
{
    // enable clk source
    SYSCLK_START();
    // enable interrupt
    SYSCLK_INT_ENABLE();
}

void sysclk_reset()
{
    sysclk_ticks=0;
    sysclk_seconds=0;
    sysclk_ticked=0;
    sysclk_seconds_ticked = 0;
}

inline uint16_t sysclk_get_ticks()
{
    return sysclk_ticks;
}

inline uint8_t sysclk_has_ticked()
{
    if( sysclk_ticked ){
	sysclk_ticked=0;
	return 1;
    }
    return 0;
}

inline uint8_t sysclk_have_seconds_ticked()
{
    if( sysclk_seconds_ticked){
	sysclk_seconds_ticked= 0;
	return 1;
    }
    return 0;
}

inline uint16_t sysclk_get_seconds()
{
    return sysclk_seconds;
}

inline void sysclk_reset_seconds()
{
    sysclk_seconds=0;
}


ISR(SYSCLK_ISR_NAME)
{
    sysclk_ticked=1;
    sysclk_ticks++;
    if( sysclk_ticks >=  SYSCLK_TICK_FREQ){
	sysclk_seconds++;
	sysclk_seconds_ticked = 1;
	sysclk_ticks=0;
    }
}


