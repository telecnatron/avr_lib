// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2017. http://telecnatron.com/
// -----------------------------------------------------------------------------
#include "mmp_cmd.h"
#include "../log.h"

void mmp_cmd_send_reply(void *handle, uint8_t status, uint8_t *data, uint8_t data_len)
{
}


void mmp_cmd_msg_handler(void *user_data, mmp_msg_t *msg)
{
    // cast pointer to mmp_cmd_ctrl_t
    mmp_cmd_ctrl_t *ctrl = (mmp_cmd_ctrl_t *)user_data;

    // XXX debug
    static unsigned int msg_count;
    LOG_INFO_FP("MSG RECEIVED: flags: 0x%02u: %u",msg->flags, ++msg_count);

    // if cmd message, echo it back to them
    if (MMP_FLAGS_IS_CMD_MSG(msg->flags)){
	// yup, it's a command-message that's been received.
	// First byte of data is cmd, second byte is used for the status,
	// therefore data must be at least 2 bytes
	LOG_INFO_FP("CMD MSG len: %02u",msg->len);
	if(msg->len >=2 ){	
	    uint8_t cmd = msg->data[0];
	    LOG_INFO_FP("CMD MSG cmd: %u", cmd);
	    if( cmd < ctrl->num_handlers ){
		// valid command, call handler
		LOG_INFO_FP("calling handler for cmd: %02u", cmd);
		// calc lengths to allow for cmd and status bytes
		uint8_t data_len = msg->len-2;
		uint8_t data_max_len = ctrl->mmp_ctrl.ctrl.data_max_len -2;
		ctrl->cmd_handler_tab[cmd](ctrl, cmd, data_len, data_max_len, msg->data+2);
	    }else{
		// invalid command
		LOG_WARN_FP("invalid command: %u", cmd);
	    }
//	    mmp_send(msg->data, msg->len, msg->flags, ((mmp_cmd_ctrl_t *)user_data)->tx_byte_fn);
	}else{
	    // data length is invalid
	}
    }else{
	// received message wasn't a command-message
    }

}

void mmp_cmd_init(mmp_cmd_ctrl_t *ctrl, uint8_t* msg_buf, uint8_t msg_buf_size, 
		  mmp_cmd_handler_t *cmd_handler_tab, uint8_t num_handlers,
		  void (*tx_byte_fn)(const char c))
{
    ctrl->tx_byte_fn = tx_byte_fn;
    ctrl->cmd_handler_tab = cmd_handler_tab;
    ctrl->num_handlers = num_handlers;
    mmp_init(&(ctrl->mmp_ctrl), msg_buf, msg_buf_size, mmp_cmd_msg_handler, ctrl);
}

void mmp_async_send(uint8_t *msg_data, uint8_t len, void (*tx_byte_fn)(const char c))
{
    mmp_send(msg_data, len, MMP_FLAGS_BIT_ASYNC, tx_byte_fn);
}


inline void mmp_cmd_rx_ch(mmp_cmd_ctrl_t *mmp_cmd_ctrl, uint8_t ch)
{
    // just call corresponding mmp function.
    mmp_rx_ch(&(mmp_cmd_ctrl->mmp_ctrl), ch);
}


inline void mmp_cmd_tick(mmp_cmd_ctrl_t *mmp_cmd_ctrl)
{
    // just call corresponding mmp function.
    mmp_tick(&(mmp_cmd_ctrl->mmp_ctrl));
}
