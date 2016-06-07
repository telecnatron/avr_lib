#ifndef _CMD_HANDLER_H
#define _CMD_HANDLER_H 1
// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2016. http://telecnatron.com/
// -----------------------------------------------------------------------------
/**
 * @file   cmd_handler.h
 * @author steves
 * @date   2016/06/05 16:49:09
 * 
 * @brief  
 * 
 */

#include "./msg.h"

#ifndef CMD_HANDLER_DEFS
#define CMD_HANDLER_DEFS
#warning "Using default defs"
//! Number of messages in the msg table
#define CMD_HANDLER_NUM_MSGS 4
//! If defined, msg index 0 is automatically defined as reset.
#define CMD_HANDLER_AUTO_RESET
#endif


//! type for handler callback functions
typedef  void (*cmd_handler_t)(uint8_t msg_num, uint8_t len, uint8_t *data);

//! 
#define CMD_HANDLER_INIT(msg_t) cmd_handler_init(msg_t, sizeof(msg_t) / sizeof(cmd_handler_t) )
void cmd_handler_init(cmd_handler_t *msg_t, uint8_t num_handlers);

//!
msg_t cmd_handler_get_msg();

//!
void cmd_handler_handler(msg_t *msg);

/** 
 * Send most recently received message back to sender
 * 
 * @param sender Pointer to function used to send the message, eg msg_uart_send()
 */
void cmd_handler_echo(void (*sender)(uint8_t *msg_data, uint8_t len));

#endif /* _CMD_HANDLER_H */

