// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2016. http://telecnatron.com/
// -----------------------------------------------------------------------------
#include "msg_uart.h"
#include "../uart/uart.h"

// globals
// msg system data
msg_ctrl_t msg_uart_ctrl;
// msg buffer
uint8_t msg_uart_buf[MSG_UART_BUF_SIZE];


void msg_uart_init(void (*handler)(msg_t *msg))
{
    msg_init(&msg_uart_ctrl, msg_uart_buf, MSG_UART_BUF_SIZE, handler);
}

void msg_uart_poll()
{
    // read chars from uart, as available
    while(GETC_AVAIL()){
	// pass char to msg
	char c=GETC();
	msg_rx_byte(&msg_uart_ctrl, (uint8_t)c);
    }
}


void msg_uart_tick()
{
    msg_tick(&msg_uart_ctrl);
}

void msg_uart_send(uint8_t *msg_data, uint8_t len)
{
    msg_send(msg_data, len, uart_putc);
}
