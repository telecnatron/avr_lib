// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2016. http://telecnatron.com/
// -----------------------------------------------------------------------------
#include <stdint.h>
#include "cmd_handler.h"

// ------------------------
// logging 
#ifdef F_CPU
// this code used when running on mcu
#include "../log.h"
#else
#define LOG_INFO_FP(fmt, msg)
#define LOG_DEBUG_FP(fmt, msg)
#endif
// ------------------------


// Globals
//! table (array) of handler functions
static cmd_handler_t *cmd_handler_tab;
//! number of handlers in  handler table
static uint8_t cmd_handler_num_handlers;
//! Pointer to the current msg
static msg_t *cmd_handler_msg;
//! Pointer to user provided reboot functions
static void (*cmd_handler_reboot_fn)();


inline void cmd_handler_init(cmd_handler_t *cmd_tab, uint8_t num_handlers, void (*reboot_fn)())
{
    cmd_handler_tab = cmd_tab;
    cmd_handler_num_handlers = num_handlers;
    cmd_handler_reboot_fn = reboot_fn;
}

msg_t cmd_handler_get_msg()
{
    return *cmd_handler_msg;
}


inline void cmd_handler_echo(void (*sender)(uint8_t *msg_data, uint8_t len))
{
    sender(cmd_handler_msg->data, cmd_handler_msg->len);
}

void cmd_handler_handler(msg_t *msg)
{
    if (MSG_LEN(msg)){
	// first byte of msg data is cmd  of the handler function
	uint8_t cmd = MSG_DATA(msg)[0];
	
	// check if this is reboot cmd
	if( cmd == CMD_HANDLER_CMD_REBOOT && MSG_LEN(msg) == 1){
	    // yup, call reboot function
	    if(cmd_handler_reboot_fn){
		LOG_INFO_FP("reset", 0);
		cmd_handler_reboot_fn();
	    }
	    // XXX never gets to here
	    return;
	}

	if( cmd < cmd_handler_num_handlers ){
	    cmd_handler_msg=msg;
	    // call handler function corresponding to this index
	    cmd_handler_t fn = *cmd_handler_tab[cmd];
	    if(fn){
		// call handler function
		fn(cmd, MSG_LEN(msg)-1, &MSG_DATA(msg)[1] );
	    }
	}else{
	    // no cmd with this cmd-number
	}
    }
}
