#ifndef UTIL_H
#define UTIL_H
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
