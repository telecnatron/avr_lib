// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing. 2015.
// $Id: util.c 427 2016-04-07 04:21:09Z steves $
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

