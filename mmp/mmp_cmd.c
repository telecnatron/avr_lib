// -----------------------------------------------------------------------------
// Copyright 2018 Stephen Stebbing. telecnatron.com
//
//    Licensed under the Telecnatron License, Version 1.0 (the “License”);
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//        https://telecnatron.com/software/licenses/
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an “AS IS” BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.
// -----------------------------------------------------------------------------   
#include "mmp_cmd.h"
#include "../log.h"

// define MSG_USE_LOGGER to have error messages logged
#ifdef MMP_CMD_LOGGING
#include "../log.h"
#define MMP_CMD_LOG(fmt, msg...) LOG_INFO_FP(fmt, msg)
#define MMP_CMD_LOG_WARN(fmt, msg...) LOG_WARN_FP(fmt, msg)
#define MMP_CMD_LOG_DEBUG(fmt, msg...) LOG_DEBUG_FP(fmt, msg)
#else
#define MMP_CMD_LOG(fmt, msg...)
#define MMP_CMD_LOG_WARN(fmt, msg...)
#define MMP_CMD_LOG_DEBUG(fmt, msg...)
#endif


void mmp_cmd_reply(void *handle, uint8_t status, uint8_t *data, uint8_t data_len)
{
    mmp_cmd_ctrl_t *ctrl = (mmp_cmd_ctrl_t *)handle;
    mmp_msg_t *msg = &(ctrl->mmp_ctrl.ctrl.msg);
    // set status
    msg->data[1]=status;
    // send reply message
    mmp_send(msg->data, data_len+2, msg->flags, ctrl->tx_byte_fn);
}


void mmp_cmd_msg_handler(void *user_data, mmp_msg_t *msg)
{
    // cast pointer to mmp_cmd_ctrl_t
    mmp_cmd_ctrl_t *ctrl = (mmp_cmd_ctrl_t *)user_data;


    // if cmd message, echo it back to them
    if (MMP_FLAGS_IS_CMD_MSG(msg->flags)){
	// yup, it's a command-message that's been received.
	// First byte of data is cmd, second byte is used for the status,
	// therefore data must be at least 2 bytes
	if(msg->len >=2 ){	
	    uint8_t cmd = msg->data[0];
	    if( cmd < ctrl->num_handlers ){
		// valid command, call handler
		// calc lengths to allow for cmd and status bytes
		uint8_t data_len = msg->len-2;
		uint8_t data_max_len = ctrl->mmp_ctrl.ctrl.data_max_len -2;
		ctrl->cmd_handler_tab[cmd](ctrl, cmd, data_len, data_max_len, msg->data+2);
	    }else{
		// invalid command, ie cmd number exceeds number of entries in cmd table
		MMP_CMD_LOG_WARN("invalid command: %u", cmd);
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
