#ifndef UTIL_WDT_H
#define UTIL_WDT_H
// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2015. http://telecnatron.com/
// $Id: wdt.h 403 2015-12-27 03:11:36Z steves $
// -----------------------------------------------------------------------------
/**
 * @file   wdt.h
 * @author Stephen Stebbing <steves@slap.ss.com>
 * @date   Thu Dec 10 11:03:12 2015
 * 
 * @brief  Watchdog timer with interrupt and no system reset. 
 *
 *<UL>
 * <LI>avr-gcc <CODE><avr/wdt.h></CODE> does not handle this, hence asm macros are provided here.</LI>
 * <LI>See atmega328 datasheet page 50</LI>
 * <LI>See: http://www.avrfreaks.net/forum/solved-need-code-watchdog-interrupt-only-configuration</LI>
 * <LI>ATMEGA8 is not supported.</LI>
 * <LI>Be sure to provide the ISR:<CODE>     ISR(WDT_vect){}</CODE></LI>
 *</UL>
 */

#include <avr/wdt.h>

#ifdef ATMEGA8
#error "Watchdog timer interrupt is not available on atmega8"
#endif

//! Enable wdt with interrupt on timeout, and without system reset
//! value parameter is one of the WDTO_xxx values as defined in avr/wdt.h
//! Note that this interrupts must be disabled when this function is called.
#define WDT_INT_ENABLE(value)   \
    __asm__ __volatile__ (  \
        "in __tmp_reg__,__SREG__" "\n\t"    \
        "wdr" "\n\t"    \
        "sts %0,%1" "\n\t"  \
        "out __SREG__,__tmp_reg__" "\n\t"   \
        "sts %0,%2" \
        : /* no outputs */  \
        : "M" (_SFR_MEM_ADDR(_WD_CONTROL_REG)), \
        "r" (_BV(_WD_CHANGE_BIT) | _BV(WDE)), \
			   "r" ((uint8_t) ((value & 0x08 ? _WD_PS3_MASK : 0x00) | \
					 _BV(WDIE) | (value & 0x07)) ) \
        : "r0"  \
    )


//! Disable wdt when it is running in interrupt-only mode.
//! Interrupts must be disabled when this macro is called
#define WDT_INT_DISABLE()  \
    __asm__ __volatile__ (  \
        "in __tmp_reg__,__SREG__" "\n\t"    \
        "wdr" "\n\t"    \
        "sts %0,%1" "\n\t"  \
        "sts %0,%2" \
        : /* no outputs */  \
        : "M" (_SFR_MEM_ADDR(_WD_CONTROL_REG)), \
        "r" (_BV(_WD_CHANGE_BIT) | _BV(WDE)), \
			   "r" ((uint8_t) (0x00)) \
        : "r0"  \
    )

#endif //ifndef UTIL_WDT_H


