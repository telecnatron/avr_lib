// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2015. http://telecnatron.com/
// $Id: boot.h 403 2015-12-27 03:11:36Z steves $
// -----------------------------------------------------------------------------
/**
 * @file   boot.h
 * @author Stephen Stebbing 
 * @date   Mon Dec 21 14:19:48 2015
 * 
 * @brief  Functions to do with bootloader.
 */


#include <stdint.h>

//! Set ISR table to be in boot section
#define BOOT_SET_ISR_BOOT()  MCUCR = _BV(IVCE); MCUCR = _BV(IVSEL)
//! Set ISR table to be in app section
#define BOOT_SET_ISR_APP()  MCUCR = _BV(IVCE); MCUCR = 0

//!See SPM_PAGESIZE in /usr/lib/avr/include/avr/ioxxx.h which is pagesize in bytes
#define BOOT_PAGE_SIZE_BYTES SPM_PAGESIZE
//! Page number of the first page of the bootloader
#define BOOT_SPACE_FIRST_PAGE BOOT_START/BOOT_PAGE_SIZE_BYTES

 
//! Write page to flash. 
//! Note that page param is an absolute memory address rather than a page number,
//! Probably best if the passed address coincides with page boundary.
uint8_t boot_write_page(uint32_t page, uint8_t *buf);

//! Erase flash page.
//! Note that page param is an absolute memory address rather than a page number,
//! Probably best if the passed address coincides with page boundary.
void boot_erase_page(uint32_t page);


