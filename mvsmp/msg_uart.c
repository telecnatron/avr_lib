// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2016. http://telecnatron.com/
// -----------------------------------------------------------------------------
#include "msg_uart.h"
#include "lib/uart/uart.h"


// Globals.
// msg control structure
static msg_ctrl_t msg_uart_ctrl;
// buffer for holding received messages
uint8_t msg_buf[MSG_UART_MSG_BUF_SIZE];


void msg_uart_init(void handler_fn(msg_t *msg))
{
    msg_init(&msg_uart_ctrl, msg_buf, sizeof(msg_buf), cmd_handler_handler);
}

void msg_uart_poll()
{
}

void msg_uart_send(uint8_t *datap, uint8_t len)
{
}
