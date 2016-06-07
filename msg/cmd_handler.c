// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2016. http://telecnatron.com/
// -----------------------------------------------------------------------------
#include <stdint.h>
#include "cmd_handler.h"
#include "../log.h"

#ifdef CMD_HANDLER_AUTO_RESET
#include "../wdt.h"
#endif

// Globals
//! table (array) of handler functions
static cmd_handler_t *cmd_handler_tab;
//! number of handlers in  handler table
static uint8_t cmd_handler_num_handlers;
//! Pointer to the current msg
static msg_t *cmd_handler_msg;

// commands numbered 0-7 are reserved, in cmd_handler_handler() this value is subtracted from
// the cmd number to arrive at the offset into the cmd_handler_tab table
// for the received command
#define CMD_HANDLER_CMD_OFFSET 8

void cmd_handler_init(cmd_handler_t *msg_tab, uint8_t num_handlers)
{
    cmd_handler_tab = msg_tab;
    cmd_handler_num_handlers = num_handlers;
}

msg_t cmd_handler_get_msg()
{
    return *cmd_handler_msg;
}

void cmd_handler_echo(void (*sender)(uint8_t *msg_data, uint8_t len))
{
    sender(cmd_handler_msg->buf, cmd_handler_msg->len);
}

void cmd_handler_handler(msg_t *msg)
{

    if (MSG_LEN(msg)){
	// first byte of msg data is index of the handler function
	uint8_t i = MSG_DATA(msg)[0];
#ifdef CMD_HANDLER_AUTO_RESET
	if( i == 0){
	    WDT_RESET();
	    // XXX never gets to here
	    return;
	}
#endif
	// calculate index into cmd_handler_tab of this command
	if (i < CMD_HANDLER_CMD_OFFSET){
	    // this is an invalid command number
	    LOG_DEBUG_FP(PSTR("Invalid command number: %u"),i);
	    return;
	}
	i = i - CMD_HANDLER_CMD_OFFSET;
	
	if( i < cmd_handler_num_handlers ){
	    cmd_handler_msg=msg;
	    // call handler function corresponding to this index
	    cmd_handler_t fn = *cmd_handler_tab[i];
	    if(fn){
		// call handler function
		fn(i, MSG_LEN(msg)-1, &MSG_DATA(msg)[1] );
	    }
	}else{
	    LOG_DEBUG_FP(PSTR("No such command number %u"),i);
	}
    }
}
