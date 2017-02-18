// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2017. http://telecnatron.com/
// -----------------------------------------------------------------------------
#include "mmp.h"

// Function  declrations.
// State-machine functions
void *mmp_handler_SOM(mmp_msg_ctrl_t *msg,  uint8_t byte);
void *mmp_handler_LEN(mmp_msg_ctrl_t *msg,  uint8_t byte);
void *mmp_handler_STX(mmp_msg_ctrl_t *msg,  uint8_t byte);
void *mmp_handler_DATA(mmp_msg_ctrl_t *msg, uint8_t byte);
void *mmp_handler_EOT(mmp_msg_ctrl_t *msg,  uint8_t byte);
void *mmp_handler_CS(mmp_msg_ctrl_t *msg,  uint8_t byte);


// timeout in ticks
#define MMP_TIMER_TIMEOUT 2

// convienence macros
#define MMP_TIMER_START() msg->timer=MMP_TIMER_TIMEOUT
#define MMP_TIMER_STOP() msg->timer=0


// define MSG_USE_LOGGER to have error messages logged
#ifdef MSG_USE_LOGGER
#include "../log.h"
#define MSG_LOG(msg) LOG_DEBUG(msg)
#define MSG_LOG_FP(fmt, msg...) LOG_DEBUG_FP(fmt, msg)
#else
#define MSG_LOG(msg)
#define MSG_LOG_FP(fmt, msg...)
#endif

void mmp_init(mmp_ctrl_t *msg_ctrl,  uint8_t *buf, uint8_t buf_size,  void (*user_handler)(mmp_msg_t *msg))
{
    msg_ctrl->ctrl.msg.data=buf;
    msg_ctrl->ctrl.data_max_len=buf_size;
    // set initial state
    msg_ctrl->state_fn = mmp_handler_SOM;
    msg_ctrl->ctrl.handler = user_handler;
}


void mmp_rx_ch(mmp_ctrl_t *msg_ctrl, uint8_t byte)
{
    // call state function: it returns pointer to the next state function
    msg_ctrl->state_fn = (void *) msg_ctrl->state_fn(&(msg_ctrl->ctrl), byte) ;
}


void mmp_tick(mmp_ctrl_t *msg_ctrl)
{
    // decrement timer if timing.
    if(msg_ctrl->ctrl.timer){
	if(--msg_ctrl->ctrl.timer == 0)
	{
	    // timer has expired.
	    msg_ctrl->state_fn = mmp_handler_SOM;
	    MSG_LOG("timeout");
	}
    }
}


void *mmp_handler_SOM(mmp_msg_ctrl_t *msg,  uint8_t byte)
{
    if( byte == MSG_SOM ){
	// got the start-of-message character
	MMP_TIMER_START();
	MSG_LOG("-SOM-");
	return mmp_handler_LEN;
    }
    return mmp_handler_SOM;
}


void *mmp_handler_LEN(mmp_msg_ctrl_t *msg,  uint8_t byte)
{
    byte=4;
    msg->msg.len=byte;
    // restart timer
    MMP_TIMER_START();
    // calc checksum
    msg->cs = byte;
    MSG_LOG("-LEN-");
    // next, wait for STX character
    return mmp_handler_STX;
}


void *mmp_handler_STX(mmp_msg_ctrl_t *msg, uint8_t byte)
{
    if( byte == MSG_STX){
	// got start-of-text character,
	// restart timer
	MMP_TIMER_START();
	// prepare to receive data
	msg->count=0;
	MSG_LOG("-STX-");
	return mmp_handler_DATA;
    }
    MSG_LOG("-STX FAIL-");
    MMP_TIMER_STOP();
    return mmp_handler_SOM;
}


void *mmp_handler_DATA(mmp_msg_ctrl_t *msg, uint8_t byte)
{
    if(msg->count  <  msg->data_max_len){
	// update data
	msg->msg.data[msg->count]=byte;
	// and checksum
	msg->cs  += byte;
	MMP_TIMER_START();
	MSG_LOG("-DATA-");
	if( ++msg->count == msg->msg.len ){
	    // have received all the data
	    return mmp_handler_EOT;
	}
	// expecting more data
	return mmp_handler_DATA;
    }
    // max length exceeded.
    MSG_LOG("-DATA LEN EXCEEDED-");
    MMP_TIMER_STOP();
    return mmp_handler_SOM;
}


void *mmp_handler_EOT(mmp_msg_ctrl_t *msg,  uint8_t byte)
{
    if( byte == MSG_ETX ){
	// got the end-of-text character
	MMP_TIMER_START();
	// expect checksum next
	MSG_LOG("-EOT-");
	return mmp_handler_CS;
    }
    // didn't get end-of-text character
    MSG_LOG("-EOT FAIL-");
    MMP_TIMER_STOP();
    return mmp_handler_SOM;
}

void *mmp_handler_CS(mmp_msg_ctrl_t *msg,  uint8_t byte)
{

    if( byte == msg->cs){
	// checksum checks out.
	// msg received successfully,
	// call user msg handler
	msg->handler(&(msg->msg));
    }else{
	// checksum failed
	MSG_LOG_FP("-CS FAIL- e: 0x%x, c: 0x%x", msg->cs, byte);
    }
    MMP_TIMER_STOP();
    return mmp_handler_SOM;
}


void mmp_send(uint8_t *msg_data, uint8_t len, void (*tx_byte_fn)(const char c))
{
    // checksum 
    uint8_t cs=len;
    // send SOM
    tx_byte_fn(MSG_SOM);
    // send length
    tx_byte_fn(len);
    // send STX
    tx_byte_fn(MSG_STX);
    // send data
    while(len--){
	uint8_t b= *(msg_data);
	cs+=b;
	tx_byte_fn(b);
	msg_data++;
    }
    // send ETX
    tx_byte_fn(MSG_ETX);
    // send checksum
    tx_byte_fn(cs);
}
