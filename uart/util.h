#ifndef UTIL_H
#define UTIL_H
// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2015. http://telecnatron.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// -----------------------------------------------------------------------------
//! Reset the baud rate. This function is intended to be used when the application
//! is using bootloader code, and need to use baud rate different to the bootloader
//! Ensure that BAUD is defined.
void uart_reset_baud();


#endif
