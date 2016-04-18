#ifndef UTIL_H
#define UTIL_H
// -----------------------------------------------------------------------------
// Copyright Stebbing Computing. 2013.
// $Id: util.h 405 2015-12-27 04:09:25Z steves $
// -----------------------------------------------------------------------------

#include <stdint.h>

/** 
 * Return the ascii character corresponding to the hexidecimal digit of the low nibble of the passed byte
 * @param b The byte
 * @return Ascii character being the hex of the low nibble
 */
char utilNibbleToHex(uint8_t b);

/** 
 * Return string of two chars being hex ascii representation of the passed byte 
 * @param b The byte
 * @param s The string to write the hex characters two, this should be at least 3 chars long,
 * to allow for the null terminator
 * @return Pointer to the string
 */
char* utilByteToHex(uint8_t b, char *s);

/** 
 * Return character being ascii for the bcd digit contained in lower nibble of bcd
 * @param bcd The bcd
 * @return The ascii character
 */
char utilBCDNibbleToAscii(uint8_t bcd);


/** 
 * Similiar to puts_P but doesn't append the annoying newline
 * 
 * @param s Pointer ot string residing in program memor
 */
void putss_P(char *s);
#endif
