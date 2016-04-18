#ifndef UTIL_H
#define UTIL_H
// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing. 2015.
// $Id: util.h 427 2016-04-07 04:21:09Z steves $
// -----------------------------------------------------------------------------
//! Reset the baud rate. This function is intended to be used when the application
//! is using bootloader code, and need to use baud rate different to the bootloader
//! Ensure that BAUD is defined.
void uart_reset_baud();


#endif
