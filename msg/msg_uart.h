#ifndef _MSG_UART_H
#define _MSG_UART_H 1
// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2016. http://telecnatron.com/
// -----------------------------------------------------------------------------
/**
 * @file   msg_uart.h
 * @author steves
 * @date   2016/06/05 15:24:56
 * 
 * @brief  
 * 
 */

#include "config.h"
#include "./msg.h"


// -----------------------------------------------------------------------------
#ifndef MSG_UART_DEFS
#define MSG_UART_DEFS
#warning "Using default definitions"

//! Size of buffer for receiving messages into, and hence max size of received message
#define MSG_UART_BUF_SIZE 32

#endif // MSG_UART_DEFS
// -----------------------------------------------------------------------------

/** 
 * 
 * 
 * @param handler 
 */
void msg_uart_init(void (*handler)(msg_t *msg));


/** 
 * 
 * 
 */
void msg_uart_poll();


/** 
 * 
 * 
 */
void msg_uart_tick();

/** 
 * 
 * 
 * @param msg_data 
 * @param len 
 * @param tx_byte_fn 
 */
void msg_uart_send(uint8_t *msg_data, uint8_t len);

#endif /* _MSG_UART_H */

