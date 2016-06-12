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

//! Received msg with the cmd number will cause reboot_fn() to be called
#define CMD_HANDLER_CMD_REBOOT 255


//! type for handler callback functions
typedef  void (*cmd_handler_t)(uint8_t msg_num, uint8_t len, uint8_t *data);

//! 
/** 
 * 
 * 
 * @param msg_t 
 * @param num_handlers 
 * @param reboot_fn 
 */
void cmd_handler_init(cmd_handler_t *cmd_tab, uint8_t num_handlers, void (*reboot_fn)());
// convenience macro: automatically calculates the num_handlers
#define CMD_HANDLER_INIT(msg_tab, reboot_fn) cmd_handler_init(msg_tab, sizeof(msg_tab) / sizeof(cmd_handler_t), reboot_fn )

//!
msg_t cmd_handler_get_msg();

//!
void cmd_handler_handler(msg_t *msg);


#endif /* _CMD_HANDLER_H */

