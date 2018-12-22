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
#include "boot.h"
#include <inttypes.h>
#include <avr/boot.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

/** 
 * Erase the flash page at the passed page address.
 * 
 * @param page The page number of the flash page that is to be erased.
 */
void boot_erase_page(uint32_t page)
{
    // Save state and disable interrupts.
    uint8_t sreg;
    sreg = SREG;
    cli();

    // wait for eeprom operation if necessary
    eeprom_busy_wait ();

    // erase the page that is about to be programmed
    boot_page_erase (page);
    boot_spm_busy_wait ();      // Wait until the memory is erased.

    // Restore interrupt state
    SREG = sreg;
}

/** 
 * Write data to flash, erase previous contents first.
 * 
 * @param page The number of the page that is to be written to
 * @param buf Pointer to the data that is to be written
 * 
 * @return 1
 */
uint8_t boot_write_page(uint32_t page, uint8_t *buf)
{
    uint16_t i;
    uint8_t sreg;

    // Disable interrupts.
    sreg = SREG;
    cli();

    // wait for eeprom operation if necessary
    eeprom_busy_wait ();

    // erase the page that is about to be programmed
    boot_page_erase (page);
    boot_spm_busy_wait ();      // Wait until the memory is erased.

    // fill buffer
    for (i=0; i<SPM_PAGESIZE; i+=2)
    {
	// Set up little-endian word.
	uint16_t w = *buf++;
	w += (*buf++) << 8;
	boot_page_fill (page + i, w);
    }

    // write buffer to flash
    boot_page_write (page);     // Store buffer in flash page.
    boot_spm_busy_wait();       // Wait until the memory is written.

    // Reenable RWW-section again. We need this if we want to jump back
    // to the application after bootloading.
    boot_rww_enable ();

    // Re-enable interrupts (if they were ever enabled).
    SREG = sreg;

    // success
    return 1;
}


