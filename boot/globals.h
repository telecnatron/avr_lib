// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2015. http://telecnatron.com/
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// -----------------------------------------------------------------------------
/**
 * @file   globals.h
 * @author Stephen Stebbing <steves@slap.ss.com>
 * @date   Mon Dec 21 14:23:07 2015
 * 
 * @brief  Global variables that are shared between bootloader and app are placed here, at the
 * start of the MCU's data space. 
 * 
 * Start of ram is 0x100, note AVR internal start of ram has 0x800000
 * added, so first RAM address is 0x800100, gcc takes care of that though,
 * so here we use 0x100
 *
 * In Makefile, we move the .data segment up so gcc puts its variables
 * above the ones defined here, by doing this in both the bootloader and
 * the application, the variables are common to both.
 * 
 * eg To reserve 11 (0xb) bytes for the global variables, use link flag:
 * 
 *    <CODE>LDFLAGS += -Wl,-section-start=.data=0x80010B</CODE>
 * 
 * Ensure that either BOOT or BOOT_APP are defined.
 */

#if !defined(BOOT) && !defined(BOOT_APP)
#error "Neither BOOT nor BOOT_APP is defined. Aborting."
#endif

//! UART control structure
//! 7 bytes in length, therefore .data should begin at 0x107
#define UART (*(uart_t*)0x100)






