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

