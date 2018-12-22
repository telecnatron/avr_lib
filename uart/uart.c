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
#include "./uart.h"

// for bootloader and standalone, the functions are defined here
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>
#include "../util/io.h"

//! checks for RX errors - these are called from the UART_RX_ISR and 
//! should not be called from user application
#define UART_IS_FRAME_ERROR()  (UART_REG_UCSRA & _BV(UART_BIT_FE))
#define UART_IS_OVERRUN_ERROR() (UART_REG_UCSRA & _BV(UART_BIT_DOR))

// -----------------------------------------------------------------------------------
#ifdef BOOT_APP
// We are running as an application that calls into the bootloaders code for uart functions.
// Functions are defined in ../boot/boot_functions.c and not here.
// -----------------------------------------------------------------------------------
#else

// We are running as bootloader or standalone application,
// functions are defined here.

void uart_init(char* buf, uint8_t buf_size){

    // init circbuf
    UART.rxbuf=buf;
    UART.rxbuf_size=buf_size;
    UART_FLUSH();
    
    // set up baud rate
    uart_set_baud();
   
    // config frame format: 8N1
    // note: this is the default 
    UART_SET_FRAME_8N1();

    // enable UART and RX interrupt
    UART_ENABLE();
    UART_RXINT_ENABLE();
}


void uart_set_baud() 
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


void uart_putc(const char c)
{
    // wait for UDR is ready
    while(! UART_TX_READY()){}
    // send char
    UART_REG_UDR=c;
}

void uart_putb(const uint8_t b)
{
    uart_putc((const char)b);
}

void uart_puts(const char* c)
{
    for(;*c!='\x0';++c){
	uart_putc(*c);
    }
}


void uart_puts_P(const char* pc)
{
    char ch;
    while(  (ch=pgm_read_byte(pc++)) != '\x0')
	uart_putc(ch);
}


void uart_write(char* buf, unsigned int size)
{
    while(size--){
	uart_putc(*(buf++));
    }
}

char uart_getc()
{
    char c;
    // Check if char is available.
    // Note that ATOMIC_BLOCK(){} ensures that interrupts
    // will not trigger whilst this code is executing.
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
	if(UART.rxbuf_count){
	    // pop char for rx buffer
	    c=UART.rxbuf[UART.rxbuf_head++];
	    if(UART.rxbuf_head == UART.rxbuf_size){
		// loop back to start
		UART.rxbuf_head=0;
	    }
	    UART.rxbuf_count--;
	}else{
	    // buffer is empty
	    c='\x0';
	}
    }
    return c;
}

//! For BOOTLOADER or standalone we define the UART RX ISR here.
ISR(UART_RX_ISR)
{
    // check for errors - if so, set error flags
    if( UART_IS_FRAME_ERROR() || UART_IS_OVERRUN_ERROR()){
    }else{
	// read received char from rx register
	char c=UART_REG_UDR;
	// push received char to rx buffer
	if(UART.rxbuf_count != UART.rxbuf_size){
	    // there is space in buffer
	    UART.rxbuf[UART.rxbuf_tail++] = c;
	    if(UART.rxbuf_tail == UART.rxbuf_size ){
		// loop back to start
		UART.rxbuf_tail = 0;
	    }
	    UART.rxbuf_count++;
	}else{
	    // buffer is full, set flag and discard received char
	}
    }
}
#endif // ifdef BOOT_APP


