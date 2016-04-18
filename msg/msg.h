#ifndef MSG_H
#define MSG_H
// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2015. http://telecnatron.com/
// $Id: msg.h 403 2015-12-27 03:11:36Z steves $
// -----------------------------------------------------------------------------
/**
 * @file   msg.h
 * @author Stephen Stebbing
 * @date   Mon Dec 21 12:39:24 2015
 * 
 * @brief  A very simple messaging system for sending and receiving messages via
 * the UART, for example. 
 * 
 * When present in the bootloader section, these functions can be called from
 * the application section. To enable this functionallity, ensure that BOOT_APP 
 * is defined when compiling msg.c
 *
 * The message consists of:<UL>
 *    <LI>Start of message byte (character) as defined by MSG_SOM</LI>
 *    <LI>Message length byte, max 256</LI>
 *    <LI>The message's data bytes</LI>
 *    <LI>End of message byte as defined by MSG_EOM</LI>
 * </UL>
 * 
 * Invalid messages are silently ignored and the system will reset back to waiting
 * for a new message.
 * An invalid message can be caused by the end-of-message character not being received 
 * when expected, or by a timeout occuring before a completed message has being received.
 *
 * To provide for timing, the msg_tick() function should be called periodically, and when
 * used with MSG_TIMEOUT_TICKS (defined in msg.c) set the timout period. 
 * With MSG_TIMEOUT_TICKS=8 and calling msg_tick() every 250ms, the timeout value is 2 seconds.
 *
 * When a message is received, the user provided callback function is called to allow access to
 * the message. The callback function has the following signature:
 *
 *   void handler(struct msg_t *msg)
 *
 * And is passed a point to a msg_t structure. msg->buf contains the message data, and msg->len
 * is the message length.
 *
 */
#include <stdint.h>

//! Byte that is sent to indicate start of message
#define MSG_SOM '~'
//! Byte that is sent to indicate end of message
#define MSG_EOM '`'

//! Bit defs for the msg_t flags field.
#define MSG_FLAG_MSG_AVAIL 1

// Convienience macros
//! True if a completed message has been received
#define MSG_IS_AVAIL(msg_ctrl_p) (msg_ctrl_p.msg.flags & _BV(MSG_FLAG_MSG_AVAIL))

//! buffer for holding the message data
typedef struct msg_t {
    uint8_t *buf;
    uint8_t buf_size;
    uint8_t timer;
    //! expected length of the message
    uint8_t len;
    //! number of characters of the message read so far
    uint8_t count;
    //
    uint8_t flags;
    //! handler fn, gets call when message has been received
    void (*handler)(struct msg_t *msg);
} msg_t;

// type for state function pointers
typedef     void *(*state_fn_t)(msg_t *msg, uint8_t byte);

typedef struct {
    state_fn_t state_fn;
    msg_t msg;
}msg_ctrl_t;


/** 
 * Initialise the messaging system.
 * 
 * @param msg_ctrl Pointer to the msg_ctrl_t structure that will be used to contain data required by the system
 * This pointer will be passed to the other msg_xxx() functions
 * @param buf The buffer that will be used to hold the message data.
 * @param buf_size The size of the buffer in bytes, maximum is 256.
 * @param handler The callback function that will be called when a message has been successfully received.
 */
void msg_init(msg_ctrl_t *msg_ctrl, uint8_t *buf, uint8_t buf_size, void (*handler)(msg_t *msg));

/** 
 * Pass a byte that has been received on the communication channel to the messaging system.
 * Eg this function would be called whenever a byte is received from the UART.
 *
 * @param msg_ctrl Pointer to control structure.
 * @param byte The received byte.
 */
void msg_rx_byte(msg_ctrl_t *msg_ctrl, uint8_t byte);

/** 
 * Send a message.
 * 
 * @param msg_data The data that is to be contained in the message. 
 * @param len The length of the data.
 * @param tx_byte_fn Function that is to be called to transmit a byte on the communication channel.
 */
void msg_send(uint8_t *msg_data, uint8_t len, void (*tx_byte_fn)(const char c));

/** 
 * Function that should be called periodically to give the messaging system its timeout ability.
 * A timeout will occur after MSG_TIMEOUT_TICKS (defined in msg.c = 8 ticks), so this function
 * could be called a rate of 250ms for a one second timeout.
 * @param msg_ctrl  Pointer to control structure.
 */
void msg_tick(msg_ctrl_t *msg_ctrl);


#endif


