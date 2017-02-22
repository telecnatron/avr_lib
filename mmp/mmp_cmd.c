// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2017. http://telecnatron.com/
// -----------------------------------------------------------------------------
#include "mmp_cmd.h"
#include "../log.h"


void mmp_cmd_msg_handler(mmp_msg_t *msg)
{
    static unsigned int msg_count;
    LOG_INFO_FP("MSG RECEIVED: flags: 0x%02u: %u",msg->flags, ++msg_count);
    // if cmd message, echo it back to them
    if (MMP_FLAGS_IS_CMD_MSG(msg->flags))
	mmp_send(msg->data, msg->len, msg->flags, uart_putc);

}

void mmp_cmd_init(mmp_cmt_ctrl_t *ctrl, uint8_t msg_buf, uint8_t msg_buf_size, 
		  void (*user_msg_handler)(mmp_msg_t *msg), void (*tx_byte_fn)(const char c))
{
    ctrl->tx_byte_fn = tx_byte_fn;
    mmp_init(&(ctrl->mmp_ctrl), msg_buf, msg_buf_size, mmp_cmd_msg_handler);
}

void mmp_async_send(uint8_t *msg_data, uint8_t len, void (*tx_byte_fn)(const char c))
{
    mmp_send(msg_data, len, MMP_FLAGS_BIT_ASYNC, tx_byte_fn);
}


