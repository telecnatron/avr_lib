#ifndef _CMD_HANDLER_H
#define _CMD_HANDLER_H 1
// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2016. http://telecnatron.com/articles/mvsmp/
// -----------------------------------------------------------------------------
/**
 * @file   cmd_handler.h
 * @author steves
 * @date   2016/06/05 16:49:09
 * 
 * @brief  A command message system that builds on msg.c. First byte of msg data represents a command number.
 * See <a href="http://telecnatron.com/articles/mvsmp/">here</a> for details.
 * 
 */

#include "./msg.h"

//! Received msg with the cmd number will cause reboot_fn() to be called
#define CMD_HANDLER_CMD_REBOOT              255
//! Async msg that is sent when bootloader is started.
#define CMD_HANDLER_CMD_BOOTLOADER_STARTED  254
//! Async msg that is sent when application is started.
#define CMD_HANDLER_CMD_APP_STARTED         253
//! These message numbers are reserved for future use
#define CMD_HANDLER_CMD_RESERVED_252        252
#define CMD_HANDLER_CMD_RESERVED_251        251
#define CMD_HANDLER_CMD_RESERVED_250        250
#define CMD_HANDLER_CMD_RESERVED_249        249
#define CMD_HANDLER_CMD_RESERVED_248        248

//! type for handler-callback functions
typedef  void (*cmd_handler_t)(uint8_t msg_num, uint8_t len, uint8_t *data);

//! 
/** 
 * Initialise the cmd_handler system.
 * @param cmd_tab Pointer to array of command-handler functions
 * @param num_handlers The number of handlers
 * @param reboot_fn Pointer to a function that causes MCU to reboot.
 */
void cmd_handler_init(cmd_handler_t *cmd_tab, uint8_t num_handlers, void (*reboot_fn)());
//! convenience macro: automatically calculates the num_handlers. XXX use with caution: needs to be called in file which defines msg_tab
#define CMD_HANDLER_INIT(msg_tab, reboot_fn) cmd_handler_init(msg_tab, sizeof(msg_tab) / sizeof(cmd_handler_t), reboot_fn )


/** 
 * Send a message with passed command numer.
 * 
 * @param cmd_num The command number.
 * @param msg_data The data that is to be contained in the message. 
 * @param data_len The length of the data.
 * @param tx_byte_fn Function that is to be called to transmit a byte on the communication channel.
 */
void cmd_handler_send(uint8_t cmd_num, uint8_t *msg_data, uint8_t data_len, void (*tx_byte_fn)(const char b));

/** 
 * Send a message that is contained in PROGMEM with passed command numer.
 * 
 * @param cmd_num The command number.
 * @param msg_data_P The data that is to be contained in the message. Must point to PROGMEM
 * @param data_len The length of the data.
 * @param tx_byte_fn Function that is to be called to transmit a byte on the communication channel.
 */
void cmd_handler_send_P(uint8_t cmd_num, uint8_t *msg_data_P, uint8_t data_len, void (*tx_byte_fn)(const char b));

//! Return pointer to (most recently) received message.
msg_t cmd_handler_get_msg();

/** 
 * Send most recently received message back to sender.
 * 
 * @param sender Pointer to function used to send the message, eg msg_uart_send()
 */
void cmd_handler_echo(void (*sender)(uint8_t *msg_data, uint8_t len));


/** 
 * Should be called when a message has been received, calls the handler function corresponding to the message's
 * command number.
 * @param msg Pointer to the received message.
 */
void cmd_handler_handler(msg_t *msg);


#endif /* _CMD_HANDLER_H */

