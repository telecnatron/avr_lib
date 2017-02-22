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


typedef struct mmp_cmt_ctrl_t{
    // mmp control structure
    mmp_ctrl_t mmp_ctrl;
    // function that is called to transmit a byte of the transmission channel
    void (*tx_byte_fn)(const char c);
}mmp_cmt_ctrl_t;



/** 
 * Initialise the command-message system
 * 
 * @param ctrl 
 * @param msg_buf 
 * @param msg_buf_size 
 * @param user_msg_handler 
 * @param tx_byte_fn 
 */
void mmp_cmd_init(mmp_cmt_ctrl_t *ctrl, uint8_t msg_buf, uint8_t msg_buf_size, 
		  void (*user_msg_handler)(mmp_msg_t *msg), void (*tx_byte_fn)(const char c));

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


void mmp_cmd_msg_handler(mmp_msg_t *msg);

#endif /* _MMP_CMD_H */

