// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2017. http://telecnatron.com/
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// -----------------------------------------------------------------------------
#include "util.h"
#include "uart.h"

void uart_reset_baud() 
{
    #include <util/setbaud.h>
    UART_REG_UBRRH = UBRRH_VALUE; 
    UART_REG_UBRRL = UBRRL_VALUE; 
#if USE_2X 
    UART_REG_UCSRA |= _BV(UART_BIT_U2X);
#else 
    UART_REG_UCSRA &=~ _BV(UART_BIT_U2X);
#endif
}

