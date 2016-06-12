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


//! type for handler callback functions
typedef  void (*cmd_handler_t)(uint8_t msg_num, uint8_t len, uint8_t *data);

//! 
#define CMD_HANDLER_INIT(msg_t) cmd_handler_init(msg_t, sizeof(msg_t) / sizeof(cmd_handler_t) )
void cmd_handler_init(cmd_handler_t *msg_t, uint8_t num_handlers);

//!
msg_t cmd_handler_get_msg();

//!
void cmd_handler_handler(msg_t *msg);


#endif /* _CMD_HANDLER_H */

