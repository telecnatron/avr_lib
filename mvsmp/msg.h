#ifndef MSG_H
#define MSG_H
// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2015. http://telecnatron.com/
// $Id: $
// -----------------------------------------------------------------------------
/**
 * @file   msg.h
 * @author Stephen Stebbing
 * @date   Mon Dec 21 12:39:24 2015
 * 
 * @brief  A very simple messaging system for sending and receiving messages, via
 * the UART for example. 
 * 
 * When present in the bootloader section, these functions can be called from
 * the application section. To enable this functionallity, ensure that BOOT_APP 
 * is defined when compiling msg.c
 *
 * The message consists of bytes (uint8_t) being:<UL>
 *    <LI>Start of message byte (character) as defined by MSG_SOM</LI>
 *    <LI>Message length byte, max 256</LI>
 *    <LI>The message's data bytes</LI>
 *    <LI>Checksum byte being the value that sums the len and data bytes to 0</LI>
 * </UL>
 * 
 * Invalid messages are silently ignored and the system will reset back to waiting
 * for a new message.
 * An invalid message can be caused by the chend-of-message character not being received 
 * when expected, or by a timeout occuring before a completed message has being received.
 *
 * To provide for timing, the <b>msg_tick()</b> function should be called periodically, and when
 * used with MSG_TIMEOUT_TICKS  set the timout period. 
 * With MSG_TIMEOUT_TICKS=8 and calling <b>msg_tick()</b> every 250ms, the timeout value is 2 seconds.
 *
 * When a message is received, the user provided callback function is called which gives access to
 * the message. The callback function has the following signature:<code>
 *
 *   void handler(struct msg_t *msg)
 *</code>
 * It is passed a pointer to a msg_t structure. msg->data contains the message data bytes, and msg->len
 * is the message length byte.
 *
 * The <code>msg_rx_char()</code> function is used by the application to pass a received byte from the HOST
 * (eg via UART) into the msg system.
 *
 * The msg_send() function is used to send a message back to the HOST. It is passed a pointer to a function
 * that does the actual sending.
 */
#include <stdint.h>
#include "config.h"

#ifndef MSG_DEFS
#warning "Using default defs"
//! Number of calls to msg_tick() with no message data received for the message to be considered as timed out.
#define MSG_TIMEOUT_TICKS 8
#endif

//! Byte that is sent to indicate start of message
#define MSG_SOM '\x2'

// Convienience macros
#define MSG_DATA(msgp) (msgp->data)
#define MSG_LEN(msgp) (msgp->len)
#define MSG_CS(sum)             (uint8_t)(256-sum) 


//! buffer for holding the message data
typedef struct msg_t {
    uint8_t *data;
    //! maximum length of buffer that data points to
    uint8_t data_max_len;
    //! timing tick count
    uint8_t timer;
    //! expected length of the message being received.
    uint8_t len;
    //! number of characters read so far of the message being received.
    uint8_t count;
    //! check sum of received message's len and data bytes
    uint8_t cs; 
    //! handler fn, gets call when message has been received
    void (*handler)(struct msg_t *msg);
} msg_t;

// type for state function pointers
typedef     void *(*state_fn_t)(msg_t *msg, uint8_t byte);

//! Structure for controlling msg reception state machine
typedef struct {
    state_fn_t state_fn;
    msg_t msg;
}msg_ctrl_t;


/** 
 * Initialise the messaging system.
 * 
 * @param msg_ctrl Pointer to the msg_ctrl_t structure that will be used to contain data required by the system.
 * This pointer will be passed to the other msg_xxx() functions
 * @param buf The buffer that will be used to hold the message data that is received.
 * @param buf_size The size of the buffer in bytes, maximum is 256.
 * @param handler The callback function that will be called when a message has been successfully received.
 */
void msg_init(msg_ctrl_t *msg_ctrl, uint8_t *buf, uint8_t buf_size, void (*handler)(msg_t *msg));

/** 
 * Pass a byte that has been received on the communication channel to the messaging system.
 * Eg this function should be called whenever a byte is received from the PC.
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
 * A timeout will occur after MSG_TIMEOUT_TICKS ticks, so if this is set to 8, then this function
 * should be called a rate of 250ms for a one second timeout.
 * @param msg_ctrl  Pointer to msg control structure.
 */
void msg_tick(msg_ctrl_t *msg_ctrl);

#endif


