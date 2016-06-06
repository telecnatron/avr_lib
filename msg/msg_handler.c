// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2016. http://telecnatron.com/
// -----------------------------------------------------------------------------
#include <stdint.h>
#include "msg_handler.h"
#include "../log.h"

#ifdef MSG_HANDLER_AUTO_RESET
#include "../wdt.h"
#endif

// Globals
//! table (array) of handler functions
static msg_handler_t *msg_handler_tab;
//! number of handlers in  handler table
static uint8_t msg_handler_num_handlers;
//! Pointer to the current msg
static msg_t *msg_handler_msg;

void msg_handler_init(msg_handler_t *msg_tab, uint8_t num_handlers)
{
    msg_handler_tab = msg_tab;
    msg_handler_num_handlers = num_handlers;
}

msg_t msg_handler_get_msg()
{
    return *msg_handler_msg;
}

void msg_handler_handler(msg_t *msg)
{

    if (MSG_LEN(msg)){
	// first byte of msg data is index of the handler function
	uint8_t i = MSG_DATA(msg)[0];
#ifdef MSG_HANDLER_AUTO_RESET
	if( i == 0){
	    WDT_RESET();
	    // XXX never gets to here
	    return;
	}
#endif
	if( i < msg_handler_num_handlers ){
	    msg_handler_msg=msg;
	    // call handler function corresponding to this index
	    msg_handler_t fn = *msg_handler_tab[i];
	    if(fn){
		// call handler function
		fn(i, MSG_LEN(msg)-1, &MSG_DATA(msg)[1] );
	    }
	}
    }
}
