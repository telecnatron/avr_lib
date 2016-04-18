// -----------------------------------------------------------------------------
// Copyright Stebbing Computing. 2013.
// $Id: util.c 405 2015-12-27 04:09:25Z steves $
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

