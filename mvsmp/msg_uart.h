#ifndef _MSG_UART_H
#define _MSG_UART_H 1
// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2016. http://telecnatron.com/
// -----------------------------------------------------------------------------
/**
 * @file   msg_uart.c
 * @author Stephen Stebbing <steves@slap.ss.com>
 * @date   Mon Jun 13 16:21:37 2016
 * 
 * @brief  Wrapper for msg.c module. Uses uart to send and received msg bytes.
 *
 */

#include "config.h"
#include "./cmd_handler.h"
#include "./msg.h"

// Configuration defines
#ifndef MSG_UART_DEFS
#define MSG_UART_DEFS
#warning "Using default defs"
#define MSG_UART_MSG_BUF_SIZE 32
#endif


/** 
 * 
 * 
 * @param handler_fn 
 */
void msg_uart_init(void handler_fn(msg_t *msg));

/** 
 * 
 * 
 */
void msg_uart_poll();

/** 
 * 
 * 
 * @param datap 
 * @param len 
 */
void msg_uart_send(uint8_t *datap, uint8_t len);


/** 
 * 
 * 
 * @param cmd_num 
 * @param msg_data 
 * @param data_len 
 */
void msg_uart_send_cmd(uint8_t cmd_num, uint8_t *msg_data, uint8_t data_len);
//! msg_data points to PROGMEM
void msg_uart_send_cmd_P(uint8_t cmd_num, uint8_t *msg_data, uint8_t data_len);
/** 
 * 
 * 
 */
void msg_uart_tick();

#endif /* _MSG_UART_H */
