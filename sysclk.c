// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2016. http://telecnatron.com/
// -----------------------------------------------------------------------------
#include "sysclk.h"
#include <avr/interrupt.h>

// tick count
uint16_t sysclk_ticks;
// seconds count
uint16_t sysclk_seconds;
// flag to indicated that clk has ticked since last call to clk_has_ticked()
uint8_t sysclk_ticked;

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

inline void sysclk_reset()
{
    sysclk_ticks=0;
    sysclk_seconds=0;
    sysclk_ticked=0;
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
	sysclk_ticks=0;
    }
}


