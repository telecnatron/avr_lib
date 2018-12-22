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
#include "util.h"
#include <avr/pgmspace.h>
#include <stdio.h>

inline char utilBCDNibbleToAscii(uint8_t bcd)
{
    return (char)bcd+'0';
}

char utilNibbleToHex(uint8_t b)
{
    b &= 0x0f;
    if(b< 10)
	return(b+'0');
    else
	return(b-10+'A');
}

char* utilByteToHex(uint8_t b, char *s)
{
    s[0]=utilNibbleToHex(b>>4);
    s[1]=utilNibbleToHex(b);
    s[2]='\x0'; // null terminator
    return s;
}


void putss_P(char *s)
{
    char c= pgm_read_byte(s);
    while(c){
        putchar(c);
        c=pgm_read_byte(++s);
    }
}

