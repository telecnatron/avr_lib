#ifndef _MMP_CMD_H
#define _MMP_CMD_H 1
// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2017. http://telecnatron.com/
// -----------------------------------------------------------------------------
/**
 * @file   mmp_cmd.h
 * @author steves
 * @date   2017/02/21 23:13:41
 * 
 * @brief  
 * 
 */

#include "config.h"
#include "mmp.h"
#include "flags.h"


//! type for message-handler callback functions
typedef  void (*mmp_cmd_handler_t)(void *handle, uint8_t cmd, uint8_t data_len, uint8_t data_max_len, uint8_t *data);


//! Control structure
typedef struct mmp_cmd_ctrl_t{
    // mmp control structure
    mmp_ctrl_t mmp_ctrl;
    // function that is called to transmit a byte on the transmission channel
    void (*tx_byte_fn)(const char c);
    // table of message-handler functions. ie pointer to an array of cmd_handler_t
    mmp_cmd_handler_t *cmd_handler_tab;
    // number of entries in cmd_handler_tab
    uint8_t num_handlers;
}mmp_cmd_ctrl_t;



/** 
 * 
 * 
 * @param ctrl 
 * @param msg_buf 
 * @param msg_buf_size 
 * @param cmd_handler_tab 
 * @param num_handlers 
 * @param tx_byte_fn 
 */
void mmp_cmd_init(mmp_cmd_ctrl_t *ctrl, uint8_t* msg_buf, uint8_t msg_buf_size, 
		  mmp_cmd_handler_t *cmd_handler_tab, uint8_t num_handlers,
		  void (*tx_byte_fn)(const char c));


/** 
 * Send a message with flags byte set to indicate that
 * message is asynchronous (ie not a response to a command message)
 */
/** 
 * 
 * 
 * @param msg_data 
 * @param len 
 * @param tx_byte_fn Callback function that is used to send the message's bytes.
 */
void mmp_async_send(uint8_t *msg_data, uint8_t len, void (*tx_byte_fn)(const char c));


void mmp_cmd_rx_ch(mmp_cmd_ctrl_t *mmp_cmd_ctrl, uint8_t ch);

void mmp_cmd_tick(mmp_cmd_ctrl_t *mmp_cmd_ctrl);

void mmp_cmd_msg_handler(void *user_data, mmp_msg_t *msg);

#endif /* _MMP_CMD_H */

