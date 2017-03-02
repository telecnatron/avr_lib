#ifndef _MMP_CMD_H
#define _MMP_CMD_H 1
// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2017. http://telecnatron.com/
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// -----------------------------------------------------------------------------
/**
 * @file   mmp_cmd.h
 * @author steves
 * @date   2017/02/21 23:13:41
 * 
 * @brief  Command-response messages and async messages on top of MMP protocol.
 * 
 */

#include "config.h"
#include "mmp.h"
#include "flags.h"


#ifndef MMP_CMD_DEFS
#warning "using default config"
// define to enable logging, undef to disable
#define MMP_CMD_LOGGING
#endif

//! type for message-handler callback functions
typedef  void (*mmp_cmd_handler_t)(void *handle, uint8_t cmd, uint8_t data_len, uint8_t data_max_len, uint8_t *data);


//! Control structure
typedef struct mmp_cmd_ctrl_t{
    //! mmp control structure
    mmp_ctrl_t mmp_ctrl;
    //! function that is called to transmit a byte on the transmission channel
    void (*tx_byte_fn)(const char c);
    //! table of message-handler functions. ie pointer to an array of cmd_handler_t
    mmp_cmd_handler_t *cmd_handler_tab;
    //! number of entries in cmd_handler_tab
    uint8_t num_handlers;
}mmp_cmd_ctrl_t;


/** 
 * Initialise the mmp_cmd system.
 * 
 * @param ctrl Pointer to control-data structure.
 * @param msg_buf Buffer for received command-messages, and possibly for response messages.
 * @param msg_buf_size Maximum size of the msg_buf
 * @param cmd_handler_tab Pointer to table of command-handler callback functions.
 * @param num_handlers The number of command-handler callback functions in the command-handler table.
 * @param tx_byte_fn Pointer to the function that will be called to transmit each byte (character) of the response message.
 */
void mmp_cmd_init(mmp_cmd_ctrl_t *ctrl, uint8_t* msg_buf, uint8_t msg_buf_size, 
		  mmp_cmd_handler_t *cmd_handler_tab, uint8_t num_handlers,
		  void (*tx_byte_fn)(const char c));


/** 
 * Send a command-reply message.
 * 
 * @param handle Pointer to the handle as obtained from call to command-handler function.
 * @param status The status resulting for command's execution. 0 for success, non-zero otherwise.
 * @param data Pointer to the data bytes of the response message
 * @param data_len Length of the response message
 */
void mmp_cmd_reply(void *handle, uint8_t status, uint8_t *data, uint8_t data_len);


/** 
 * Send a message with flags byte set to indicate that
 * message is asynchronous (ie not a response to a command message)
 * Used, for example, for MCU button clicks.
 * 
 * @param msg_data Pointer to the message's data.
 * @param len Lenght of the message's data.
 * @param tx_byte_fn Callback function that is used to send the message's bytes.
 */
void mmp_async_send(uint8_t *msg_data, uint8_t len, void (*tx_byte_fn)(const char c));


/** 
 * Called to pass a character (byte) that has been received on the communication channel onto the mmp_cmd system.
 * 
 * @param msg_cmd_ctrl Pointer to the mmp_cmd_ctrl_t structure.
 * @param ch The character (byte) that has been received.
 */
void mmp_cmd_rx_ch(mmp_cmd_ctrl_t *mmp_cmd_ctrl, uint8_t ch);

/** 
 * Should be called periodically so message reception timeouts can be detected.
 * Default implementation calls this at about 1000Hz
 * @param mmp_cmd_ctrl Pointer to the mmp_cmd_ctrl_t structure.
 */
void mmp_cmd_tick(mmp_cmd_ctrl_t *mmp_cmd_ctrl);

#if 0
// XXX internal function, declaration not required here.
void mmp_cmd_msg_handler(void *user_data, mmp_msg_t *msg);
#endif

#endif /* _MMP_CMD_H */

