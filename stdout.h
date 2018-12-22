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
/**
 * @file   stdout.h
 * @author Stephen Stebbing 
 * @date   Sat Aug 30 21:08:39 2014
 * 
 * @brief  This include file eases setup of stdout stream for use of printf type functions.
 * It uses  macro PUTC(c) to output a characer- it must have been defined prior to including this file.
 */

#include <stdio.h>
#include "./uart/uart.h"

/** 
 * Macro to initilises the stdout global varible to use the function provided here. 
 * Should be called prior to using any printf-style functions.
 * 
 */
#define STDOUT_INIT() (stdout=&stdoutput)

// setup stdout
/** 
 * Function that is called by printf-style functions to write a character to stdout.
 * Ensure that PUTC() is defined, this is used to actually write the character to the output stream.
 * 
 * @param c The character that is to be written
 * @param stream The stream to which character is to be written, usually stdout
 * 
 * @return 0
 */
static int stdio_putc(char c, FILE *stream)
{
    if (c == '\n')
        stdio_putc('\r', stream);
    PUTC(c);
    return 0;
}
//! Configure stdout to use our stdio_putc()
static FILE stdoutput = FDEV_SETUP_STREAM(stdio_putc, NULL, _FDEV_SETUP_WRITE);
