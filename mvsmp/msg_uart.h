#ifndef _MSG_UART_H
#define _MSG_UART_H 1
// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2015. See: http://telecnatron.com/articles/mvsmp/
// -----------------------------------------------------------------------------
/**
 * @file   msg_uart.c
 * @author Stephen Stebbing <steves@slap.ss.com>
 * @date   Mon Jun 13 16:21:37 2016
 * 
 * @brief  Wrapper for msg.c module. Uses uart to send and received msg bytes.
 */

#include "config.h"
#include "./cmd_handler.h"
#include "./msg.h"

// Configuration defines
#ifndef MSG_UART_DEFS
#define MSG_UART_DEFS
#warning "Using default defs"
//! Maximum size of a received message. Messages longer than this are dropped.
#define MSG_UART_MSG_BUF_SIZE 32
#endif

/** 
 * Initialise the msg system to use the uart. Note that uart this function 
 * does not initialise the uart.
 * @param handler_fn Pointer to the callback function to be used by the message system
 * on receipt of a message.
 */
void msg_uart_init(void handler_fn(msg_t *msg));

/** 
 * Read character from uart (if available) and append to the message that is being received.
 * 
 */
void msg_uart_poll();

/** 
 * Send passed message via the uart.
 * 
 * @param datap Message data
 * @param len Length of message data.
 */
void msg_uart_send(uint8_t *datap, uint8_t len);


/** 
 * Send passed command-message via the uart.
 * 
 * @param cmd_num The command number of the message that is to be sent
 * @param msg_data Pointer to the message data
 * @param data_len Lenght of the message data
 */
void msg_uart_send_cmd(uint8_t cmd_num, uint8_t *msg_data, uint8_t data_len);
//! As per msg_uart_send_cmd() but msg_data points to PROGMEM
void msg_uart_send_cmd_P(uint8_t cmd_num, uint8_t *msg_data, uint8_t data_len);

//! Wrapper for msg_tick()
void msg_uart_tick();

#endif /* _MSG_UART_H */
