#ifndef UART_H
#define UART_H
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
 * @file   uart.h
 * @author Stephen Stebbing 
 * @date   Mon Dec 21 15:16:26 2015
 * 
 * @brief  UART library. Uses interrupted driven RX and polled TX. RX uses circular buffer.
 * 
 */

#include <stdint.h>
#include <avr/io.h>

#include "../boot/boot.h"

// processor specific include
#if defined(ATMEGA328) || defined(ATMEGA168) || defined(ATMEGA88) || defined(ATMEGA48)
  #include "./uart_atmega328.h"
#elif defined(ATTINY2313)
  #include "./uart_attiny2313.h"
#elif defined(ATMEGA8)
 #include "./uart_atmega8.h"
#else 
  #error "No MCU type for uart is defined."
#endif

//! baud rate - define this prior to change. eg in Makefile
//! this is default value.
#ifndef BAUD
#define BAUD 19200
#warning "BAUD is not defined, using default "$(BAUD)
#endif

//! uart control structure.
typedef struct {
    //! rx circular buffer
    char*  rxbuf;
    uint8_t rxbuf_size;  //! size of queue in bytes.
    uint8_t rxbuf_head;  //! index into buf of head of queue.
    uint8_t rxbuf_tail;  //! index into buf of tail of queue.
    uint8_t rxbuf_count; //! number of characters currently in queue.
} uart_t;


//! Initialise the global variable 'uart_t uart' 
//! Note that this should always be accessed via the UART macro.
#if defined(BOOT) || defined(BOOT_APP)
  // Globals are shared between bootloader and app
  #include "../boot/globals.h"
#else
  // We're running as standalone application. Declare the global variable here.
  volatile uart_t uart;
  #define UART uart
#endif


//! Enable/disable the UART on RX and TX or both
//! Enable RX
#define UART_RX_ENABLE()  UART_REG_UCSRB |=   _BV(UART_BIT_RXEN ) 
//! Disable RX
#define UART_RX_DISABLE() UART_REG_UCSRB &=~  _BV(UART_BIT_RXEN ) 
//! Enable TX
#define UART_TX_ENABLE()  UART_REG_UCSRB |=  _BV(UART_BIT_TXEN ) 
//! Disable TX
#define UART_TX_DISABLE() UART_REG_UCSRB &=~ _BV(UART_BIT_RXEN ) 
//! Enable both RX and TX
#define UART_ENABLE()  UART_REG_UCSRB |=  ( _BV(UART_BIT_RXEN ) | _BV(UART_BIT_TXEN ) )
//#define UART_ENABLE()  UART_REG_UCSRB |= (1 << UART_BIT_RXEN ) | (1 << UART_BIT_TXEN ) 
//! Disable both RX and TX
#define UART_DISABLE() UART_REG_UCSRB &=~ ( _BV(UART_BIT_RXEN ) | _BV(UART_BIT_TXEN ) )

//! Return true if UDR is empty, false otherwise - True indicates that new character can be transmitted.
#define UART_TX_READY()  ( UART_REG_UCSRA & _BV( UART_BIT_UDRE ) ) 
//! Character has been received and is available for reading for adding to circular buffer
#define UART_RX_AVAILABLE() ( UART_REG_UCSRA & ( _BV(UART_BIT_RXC)) )
//! enable RX interrupt
#define UART_RXINT_ENABLE() (UART_REG_UCSRB |= _BV(UART_BIT_RXCIE))
//! disable RX interrupt
#define UART_RXINT_DISABLE() (UART_REG_UCSRB &=~ _BV(UART_BIT_RXCIE))



// convinence macros

//! Transmit passed character
#define PUTC(c) uart_putc(c)
//! Transmit passed string
#define PUTS(s) uart_puts(s)
//! Transmit passed progmem string
#define PUTS_P(sp) uart_puts_P(sp)

//! Pop received char from rx circular buffer
#define GETC() uart_getc()
//! Non zero if a rx char is availabe in the buffer
#define UART_CHAR_AVAIL() (UART.rxbuf_count) 
#define GETC_AVAIL() UART_CHAR_AVAIL()

//! Discard any received character that are in the rx circular buffer
#define UART_FLUSH() (UART.rxbuf_head = UART.rxbuf_tail = UART.rxbuf_count = 0)

//! set baud rate - BAUD mast be defined. eg \#define BAUD=9600, or -D BAUD=9600 in Makefile,
//! or define in config.h, or define just prior to call to uart_set_baud()
void uart_set_baud();

/** 
 * Initialise the uart. 
 * Sets baud, frame, initilises rx circular buffer, enables uart, enables RX ISR
 * @param buf      Pointer to char array that will be used for rx circular buffer data
 * @param buf_size Number of bytes (max) in the rx buffer
 */
void uart_init(char* buf, uint8_t buf_size);

/** 
 * Transmit passed character. Wait until ready.
 * @param c The character to be transmitted
 */
void uart_putc(const char c);


/** 
 * Transmit the string pointed to by the passed pointer
 * Wait until ready.
 * @param c Pointer to the string.
 */
void uart_puts(const char* c);

/** 
 * Transmit the string contained in progmem that is pointed to by the passed pointer
 * Wait until ready.
 * @param pc Pointer to the string.
 */
void uart_puts_P(const char* pc);

/** 
 * Transmit the character data pointed to by the passed pointer.
 * @param buf The data to be transmitted
 * @param size The number of chars to transmimt
 */
void uart_write(char* buf, unsigned int size);

/** 
 * Return next character from rx buffer. User should ensure that one is available prior to calling.
 * @return The character that was popped, or undefined if buffer was empty.
 */
char uart_getc();


#endif
