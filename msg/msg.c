// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2015. http://telecnatron.com/
// -----------------------------------------------------------------------------
#include "msg.h"
#include <stddef.h>
#include <avr/io.h>

// -----------------------------------------------------------------------------------
#ifdef BOOT_APP
// We are running as an application that calls into the bootloaders code for uart functions.
// Functions are defined in ../boot/boot_functions.c and not here.
// -----------------------------------------------------------------------------------
#else
// We are running as bootloader or standalone application,
// functions are defined here.

// declrations for event handler functions
void *msg_handler_SOM(msg_t *msg, uint8_t byte);
void *msg_handler_LEN(msg_t *msg, uint8_t byte);
void *msg_handler_DATA(msg_t *msg, uint8_t byte);
void *msg_handler_EOM(msg_t *msg, uint8_t byte);

// convienience macros:
// call function for current state
#define MSG_CALL(event, byte)   msg_ctrl->state_fn=msg_ctrl->state_fn(&(msg_ctrl->msg), event, byte) 
// (re)start timer
#define MSG_TIMER_START()       msg_timer_set(msg, MSG_TIMEOUT_TICKS)

void msg_init(msg_ctrl_t *msg_ctrl,  uint8_t *buf, uint8_t buf_size,  void (*handler)(msg_t *msg))
{
    msg_ctrl->msg.buf=buf;
    msg_ctrl->msg.buf_size=buf_size;
    // set initial state
    msg_ctrl->state_fn = (state_fn_t)msg_handler_SOM;
    msg_ctrl->msg.handler=handler;
}

void msg_rx_byte(msg_ctrl_t *msg_ctrl, uint8_t byte)
{
    // call state function, it returns pointer to the next state function
    msg_ctrl->state_fn=msg_ctrl->state_fn(&(msg_ctrl->msg), byte) ;
}

void msg_send(uint8_t *msg_data, uint8_t len, void (*tx_byte_fn)(const char b))
{
    tx_byte_fn(MSG_SOM);
    tx_byte_fn(len);
    while(len--){
	tx_byte_fn(*(msg_data++));
    }
    tx_byte_fn(MSG_EOM);

}

void msg_tick(msg_ctrl_t *msg_ctrl)
{
    if(msg_ctrl->msg.timer > 0){
	// timer is running, decrement it
	--(msg_ctrl->msg.timer);
	if(!msg_ctrl->msg.timer){
	    // timer has expired.
	    msg_ctrl->state_fn = (state_fn_t)msg_handler_SOM;
	}
    }
}

void msg_timer_set(msg_t *msg, uint8_t ticks)
{
    msg->timer=ticks;
}

void *msg_handler_SOM(msg_t *msg, uint8_t byte)
{
    if( !(msg->flags & _BV(MSG_FLAG_MSG_AVAIL)) && (byte == MSG_SOM )){
	// got the start-of-message character
	MSG_TIMER_START();
	return (void *)msg_handler_LEN;
    }
    return (void *)msg_handler_SOM;
}

void *msg_handler_LEN(msg_t *msg, uint8_t byte)
{
    msg->len=byte;
    // restart timer
    MSG_TIMER_START();
    // prepare to receive the message data
    msg->count=0;
    return (void *)msg_handler_DATA;
}

void *msg_handler_DATA(msg_t *msg, uint8_t byte)
{
    if(msg->count  <  msg->buf_size){
	msg->buf[msg->count]=byte;
	MSG_TIMER_START();
	if( ++msg->count == msg->len ){
	    // have received all the data
	    return (void *)msg_handler_EOM;
	}
	return (void *)msg_handler_DATA;
    }
    return (void *)msg_handler_SOM;
}

void *msg_handler_EOM(msg_t *msg, uint8_t byte)
{
    if( (byte == MSG_EOM )){
	// got the end-of-message character,
	// we have a complete message.
	msg->flags |= _BV(MSG_FLAG_MSG_AVAIL);
	// call handler function
	msg->handler(msg);
	msg->flags &=~ _BV(MSG_FLAG_MSG_AVAIL);
    }
    // did not receive the expected end-of-message character
    return (void *)msg_handler_SOM;
}
#endif
