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


inline void msg_uart_init(void handler_fn(msg_t *msg))
{
    msg_init(&msg_uart_ctrl, msg_buf, sizeof(msg_buf), handler_fn);
}

void msg_uart_poll()
{
    // read all available chars from uart 
    while(GETC_AVAIL()){
	msg_rx_byte(&msg_uart_ctrl, (uint8_t)GETC());
    }
}

inline void msg_uart_send(uint8_t *datap, uint8_t len)
{
    msg_send(datap, len, uart_putc);
}


void msg_uart_send_cmd(uint8_t cmd_num, uint8_t *msg_data, uint8_t data_len)
{
    cmd_handler_send(cmd_num, msg_data, data_len, uart_putc);
}

void msg_uart_send_cmd_P(uint8_t cmd_num, uint8_t *msg_data, uint8_t data_len)
{
    cmd_handler_send_P(cmd_num, msg_data, data_len, uart_putc);
}


inline void msg_uart_tick()
{
    msg_tick(&msg_uart_ctrl);
}
