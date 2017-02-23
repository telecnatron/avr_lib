#ifndef _MMP_H
#define _MMP_H 1
// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2017. http://telecnatron.com/
// -----------------------------------------------------------------------------
/**
 * @file   mmp.h
 * @author steves
 * @date   2017/02/18 01:54:54
 * 
 * @brief  
 * 
 */
#include "config.h"
#include <stdint.h>

// 


#ifndef MMP_DEFS
#define MMP_DEFS
#warning "Using default config"d
// Timeout in ticks
#define MMP_TIMER_TIMEOUT 2
// Define MMP_NO_REBOOT to disable the reboot message
#undef MMP_NO_REBOOT
#endif

// Message indicator characters:
// start of message
#define MSG_SOM '\1'
// start of text
#define MSG_STX '\2'
// end of text
#define MSG_ETX '\3'

//! buffer for holding the message data
typedef struct mmp_msg_t {
    //! expected length of the message being received, or the length of a successfully received msg.
    uint8_t len;
    //! flags as specified by higher level protocols, not significant here.
    uint8_t flags;
    //! The message data
    uint8_t *data;
} mmp_msg_t;

typedef struct mmp_msg_ctrl_t {
   //! 
    mmp_msg_t msg;
    //! maximum length of buffer that data points to
    uint8_t data_max_len;
    //! timing tick count
    //uint8_t timer;
    //XXX DEVEL: use uint16 for higher max timeout
    uint16_t timer;
    //! number of characters read so far of the message being received.
    uint8_t count;
    //! crc check sum of received message's len and data bytes
    uint8_t cs; 
    //! handler fn, gets call when message has been received
    void (*handler)(void *user_data, mmp_msg_t *msg);
    //! user data, gets passed to the handler function
    void *user_data;
} mmp_msg_ctrl_t;


//! struct for holding state-machine and msg data.
typedef struct {
    //! Pointer to state-machine current handler function
    void *(*state_fn)(mmp_msg_ctrl_t *msg, uint8_t byte);
    mmp_msg_ctrl_t ctrl;
} mmp_ctrl_t;


/** 
 * Initialise the messaging system.
 * 
 * @param msg_ctrl Pointer to the mmp_msg_ctrl_t structure that will be used to contain data required by the system.
 * This pointer will be passed to the other msg_xxx() functions
 * @param buf The buffer that will be used to hold the message data that is received.
 * @param buf_size The size of the buffer in bytes, maximum is 256.
 * @param user_msg_handler The callback function that will be called when a message has been successfully received.
 * @param user_data
 */
void mmp_init(mmp_ctrl_t *msg_ctrl,  uint8_t *buf, uint8_t buf_size,  void (*user_handler)(void *user_data, mmp_msg_t *msg), void *user_data);


void mmp_tick(mmp_ctrl_t *mmp_ctrl);

/** 
 * 
 * 
 * @param msg_ctrl 
 * @param ch
 */
void mmp_rx_ch(mmp_ctrl_t *mmp_ctrl, uint8_t ch);

/** 
 * Send a message.
 * 
 * @param msg_data The data that is to be contained in the message. 
 * @param len The length of the data.
 * @param tx_byte_fn Function that is to be called to transmit a byte on the communication channel.
 */
void mmp_send(uint8_t *msg_data, uint8_t len, uint8_t flags, void (*tx_byte_fn)(const char c));

#endif /* _MMP_H */

