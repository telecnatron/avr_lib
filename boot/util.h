#ifndef BOOT_UTIL_H
#define BOOT_UTIL_H
// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2015. http://telecnatron.com/
// $Id: util.h 403 2015-12-27 03:11:36Z steves $
// 
// -----------------------------------------------------------------------------
#include <stdint.h>
#include <stddef.h>

/**
 * 
 * @brief  Utilities that can be shared between bootloader application 
 *  and normal application.
 * 
 */




//! see _crc_xmodem_update
uint16_t bu_crc_xmodem_update (uint16_t __crc, uint8_t __data);

//! See eeprom_read_block()
void bu_eeprom_read_block(void * __dst, const void *__src, size_t __n);

//! See eeprom_update_block()
void bu_eeprom_update_block (const void *__src, void *__dst, size_t __n);
#endif




