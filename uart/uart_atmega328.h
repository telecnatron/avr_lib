#ifndef UART_ATTINY2313_H
#define UART_ATTINY2313_H
// -----------------------------------------------------------------------------
// Copyright Stebbing Computing. 2013.
// $Id: uart_atmega328.h 403 2015-12-27 03:11:36Z steves $
// -----------------------------------------------------------------------------
/**
 * @file   uart_atmega328.h
 * @author Stephen Stebbing
 * @date   Mon Dec 21 15:18:07 2015
 * 
 * @brief  Include file for uart macros for atmega328 processors, also for atmega168,88,48
 * 
 */
// register defs
#define UART_REG_UDR UDR0
#define UART_REG_UCSRA UCSR0A
#define UART_REG_UCSRB UCSR0B
#define UART_REG_UBRRH UBRR0H
#define UART_REG_UBRRL UBRR0L

// register bits
#define UART_BIT_U2X U2X0
#define UART_BIT_UDRE UDRE0
#define UART_BIT_UDRIE UDRIE0
#define UART_BIT_RXC RXC0
#define UART_BIT_RXCIE RXCIE0
#define UART_BIT_FE FE0
#define UART_BIT_DOR DOR0
#define UART_BIT_RXEN RXEN0
#define UART_BIT_TXEN TXEN0

// RX ISR name
#define UART_RX_ISR USART_RX_vect 

// macro sets frame to async, 8N1
#define UART_SET_FRAME_8N1()  UCSR0B &=~ _BV(UCSZ02);\
    UCSR0C |= (_BV(UCSZ01) | _BV(UCSZ00));\
    UCSR0C &=~ _BV(USBS0); \
    UCSR0C &=~ (_BV(UPM01) | _BV(UPM00))

#endif
