#ifndef _MSG_UART_H
#define _MSG_UART_H 1
// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2016. http://telecnatron.com/
// -----------------------------------------------------------------------------
/**
 * @file   msg_uart.h
 * @author steves
 * @date   2016/06/12 14:42:59
 * 
 * @brief  
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
#endif /* _MSG_UART_H */

