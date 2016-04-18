#ifndef UART_ATMEGA8_H
#define UART_ATMEGA8_H
// -----------------------------------------------------------------------------
// Copyright Stebbing Computing. 2013.
// $Id: uart_atmega8.h 403 2015-12-27 03:11:36Z steves $
// -----------------------------------------------------------------------------
/**
 * @file   uart_atmega328.h
 * @author Stephen Stebbing
 * @date   Mon Dec 21 15:18:07 2015
 * 
 * @brief  Include file for uart macros for atmega8 processors
 * 
 */

// register defs
#define UART_REG_UDR UDR
#define UART_REG_UCSRA UCSRA
#define UART_REG_UCSRB UCSRB
#define UART_REG_UBRRH UBRRH
#define UART_REG_UBRRL UBRRL

// register bits
#define UART_BIT_U2X U2X
#define UART_BIT_UDRE UDRE
#define UART_BIT_UDRIE UDRIE
#define UART_BIT_RXC RXC
#define UART_BIT_RXCIE RXCIE
#define UART_BIT_FE FE
#define UART_BIT_DOR DOR
#define UART_BIT_RXEN RXEN
#define UART_BIT_TXEN TXEN

//  RX ISR name
#define UART_RX_ISR USART_RXC_vect
#endif
