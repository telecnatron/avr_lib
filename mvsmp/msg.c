// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2015. http://telecnatron.com/
// -----------------------------------------------------------------------------
#include "msg.h"
#include <stddef.h>

// -----------------------------------------------------------------------------------
#ifdef BOOT_APP
// We are running as an application that calls into the bootloaders code for uart functions.
// Functions are defined in ../boot/boot_functions.c and not here.
// -----------------------------------------------------------------------------------
#else
// We are running as bootloader or standalone application,
// functions are defined here.
// declrations for event handler functions
void *msg_handler_SOM(msg_t *msg,  uint8_t byte);
void *msg_handler_LEN(msg_t *msg,  uint8_t byte);
void *msg_handler_DATA(msg_t *msg, uint8_t byte);
void *msg_handler_EOM(msg_t *msg,  uint8_t byte);

// convienience macros:
// call function for current state
#define MSG_CALL(event, byte)   msg_ctrl->state_fn=msg_ctrl->state_fn(&(msg_ctrl->msg), event, byte) 
// (re)start timer
#define MSG_TIMER_START()       msg_timer_set(msg, MSG_TIMEOUT_TICKS)
#define MSG_CS(sum)             (256-sum)

// test/debug
#ifdef MSG_DEBUG
#include <stdio.h>
#define _MSG_LOG(fmt, msg...) printf(fmt, msg)
#else
#define _MSG_LOG(fmt, msg...)
#endif


void msg_init(msg_ctrl_t *msg_ctrl,  uint8_t *buf, uint8_t buf_size,  void (*handler)(msg_t *msg))
{
    msg_ctrl->msg.data=buf;
    msg_ctrl->msg.data_max_len=buf_size;
    // set initial state
    msg_ctrl->state_fn = (state_fn_t)msg_handler_SOM;
    msg_ctrl->msg.handler=handler;
}

void msg_rx_byte(msg_ctrl_t *msg_ctrl, uint8_t byte)
{
    // call state function: it returns pointer to the next state function
    _MSG_LOG("rx: 0x%02x sf: %p, \n",byte, msg_ctrl->state_fn);
    msg_ctrl->state_fn = msg_ctrl->state_fn(&(msg_ctrl->msg), byte) ;
    
}

void msg_send(uint8_t *msg_data, uint8_t len, void (*tx_byte_fn)(const char b))
{
    // checksum 
    uint8_t cs=len;
    // send SOM
    tx_byte_fn(MSG_SOM);
    // send length
    tx_byte_fn(len);
    // send data
    while(len--){
	uint8_t b= *(msg_data);
	cs+=b;
	tx_byte_fn(b);
	msg_data++;
    }
    // send checksum
    tx_byte_fn(MSG_CS(cs));
}

void msg_tick(msg_ctrl_t *msg_ctrl)
{
    if(msg_ctrl->msg.timer > 0){
	// timer is running, decrement it
	--(msg_ctrl->msg.timer);
	if(!msg_ctrl->msg.timer){
	    // timer has expired.
	    msg_ctrl->state_fn = (state_fn_t)msg_handler_SOM;
	    _MSG_LOG("timeout%c",'\n');
	}
    }
}

void msg_timer_set(msg_t *msg, uint8_t ticks)
{
    msg->timer=ticks;
}

void *msg_handler_SOM(msg_t *msg, uint8_t byte)
{
    if( byte == MSG_SOM ){
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
    // prepare to receive the message data:
    msg->cs = msg->len;
    msg->count=0;
    return (void *)msg_handler_DATA;
}

void *msg_handler_DATA(msg_t *msg, uint8_t byte)
{
    if(msg->count  <  msg->data_max_len){
	// update data
	msg->data[msg->count]=byte;
	// and checksum
	msg->cs += byte;
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
    // byte is the received checksum char
    if ( byte == MSG_CS(msg->cs)){
	// checksums matched, we have a complete message,
	// call handler function
	msg->handler(msg);
    }else{
	_MSG_LOG("invalid checksum: got %u, expected %u\n",byte, MSG_CS(msg->cs));
    }
    return (void *)msg_handler_SOM;
}
#endif
