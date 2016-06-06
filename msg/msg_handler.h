#ifndef _MSG_HANDLER_H
#define _MSG_HANDLER_H 1
// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2016. http://telecnatron.com/
// -----------------------------------------------------------------------------
/**
 * @file   msg_handler.h
 * @author steves
 * @date   2016/06/05 16:49:09
 * 
 * @brief  
 * 
 */

#include "./msg.h"

#ifndef MSG_HANDLER_DEFS
#define MSG_HANDLER_DEFS
#warning "Using default defs"
//! Number of messages in the msg table
#define MSG_HANDLER_NUM_MSGS 4
//! If defined, msg index 0 is automatically defined as reset.
#define MSG_HANDLER_AUTO_RESET
#endif


//! type for handler callback functions
typedef  void (*msg_handler_t)(uint8_t msg_num, uint8_t len, uint8_t *data);

//! 
#define MSG_HANDLER_INIT(msg_t) msg_handler_init(msg_t, sizeof(msg_t) / sizeof(msg_handler_t) )
void msg_handler_init(msg_handler_t *msg_t, uint8_t num_handlers);

//!
msg_t msg_handler_get_msg();

//!
void msg_handler_handler(msg_t *msg);



#endif /* _MSG_HANDLER_H */

