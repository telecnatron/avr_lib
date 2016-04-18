// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2015. http://telecnatron.com/
// $Id: util.c 403 2015-12-27 03:11:36Z steves $
// -----------------------------------------------------------------------------
#include "./util.h"
#include <avr/eeprom.h>
#include <util/crc16.h>

// -----------------------------------------------------------------------------------
#ifdef BOOT_APP
// We are running as an application that calls into the bootloaders code for uart functions.

// functions are defined in boot_functions.h and not here.
#include "../boot/boot_functions.h"

// -----------------------------------------------------------------------------------
#else

uint16_t bu_crc_xmodem_update (uint16_t __crc, uint8_t __data)
{
    return _crc_xmodem_update(__crc,__data);
}


void bu_eeprom_read_block(void *__dst, const void *__src, size_t __n)
{
    eeprom_read_block(__dst, __src, __n);
}

void bu_eeprom_update_block (const void *__src, void *__dst, size_t __n)
{
    eeprom_update_block(__src, __dst, __n);
}

// -----------------------------------------------------------------------------------
#endif



