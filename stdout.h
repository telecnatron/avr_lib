// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing. 2013 - 2016.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// -----------------------------------------------------------------------------

/**
 * @file   stdout.h
 * @author Stephen Stebbing 
 * @date   Sat Aug 30 21:08:39 2014
 * 
 * @brief  This include file eases setup of stdout stream for use of printf type functions.
 * It uses  macro PUTC(c) to output a characer- it must have been defined prior to including this file.
 * In the main() function include the line (or call the macro STDOUT_INIT()):
 *
 *       stdout = &stdoutput;
 *
 * Then printf will be available.
 */

#include <stdio.h>
#include "./uart/uart.h"

#define STDOUT_INIT() (stdout=&stdoutput)

// setup stdout -
static int stdio_putc(char c, FILE *stream)
{
    if (c == '\n')
        stdio_putc('\r', stream);
    PUTC(c);
    return 0;
}
// FILE stream to handle stdout output
static FILE stdoutput = FDEV_SETUP_STREAM(stdio_putc, NULL, _FDEV_SETUP_WRITE);
