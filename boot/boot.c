// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2015. http://telecnatron.com/
// $Id: boot.c 403 2015-12-27 03:11:36Z steves $
// -----------------------------------------------------------------------------
#include "boot.h"
#include <inttypes.h>
#include <avr/boot.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

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


