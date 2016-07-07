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
#define CMD_HANDLER_CMD_REBOOT              255
#define CMD_HANDLER_CMD_BOOTLOADER_STARTED  254
#define CMD_HANDLER_CMD_APP_STARTED         253


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


/** 
 * Send a message with passed command numer.
 * 
 * @param cmd_num The command number.
 * @param msg_data The data that is to be contained in the message. 
 * @param len The length of the data.
 * @param tx_byte_fn Function that is to be called to transmit a byte on the communication channel.
 */
void cmd_handler_send(uint8_t cmd_num, uint8_t *msg_data, uint8_t data_len, void (*tx_byte_fn)(const char b));

//! Send a message that is contained in PROGMEM
void cmd_handler_send_P(uint8_t cmd_num, uint8_t *msg_data_P, uint8_t data_len, void (*tx_byte_fn)(const char b));

//!
msg_t cmd_handler_get_msg();

/** 
 * Send most recently received message back to sender
 * 
 * @param sender Pointer to function used to send the message, eg msg_uart_send()
 */
void cmd_handler_echo(void (*sender)(uint8_t *msg_data, uint8_t len));


/** 
 * 
 * 
 * @param msg 
 */
void cmd_handler_handler(msg_t *msg);



#endif /* _CMD_HANDLER_H */

