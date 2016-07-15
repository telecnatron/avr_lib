// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2015. http://telecnatron.com/
// $Id: boot_functions.c 395 2015-12-25 18:35:16Z steves $
// -----------------------------------------------------------------------------
/**
 * @file   boot_functions.c
 * @author Stephen Stebbing 
 * @date   Mon Dec 21 14:09:38 2015
 * 
 * @brief  Function stubs for use when the application is calling functions 
 * provided by the bootloader.
 * 
 * The stubs simply jump to the corresponding function as defined in the jump table
 * in the .boot_ftab section in boot_ftab.S
 *
 * Note that the addresses are divided by 2 to turn them into the required word address.
 * That gcc does not do this automatically could be a considered a bug?.
 *
 * Eg to call the function located at offset 2 in the jump table:
 * 
 *     ((PF_VOID)((BOOT_FTAB_START + 0x2)/2))();
 */

#include <stdint.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "../uart/uart.h"
#include "../mvsmp/msg.h"

//! This is the address of the boot exported-function table,
//! and should be defined in Makefile
//! see boot_fstab.S for section definition.
// #define BOOT_FTAB_START 0x7FCE
#ifndef BOOT_FTAB_START
  #error "BOOT_FTAB_START must be defined to indicate starting address or the boot-function table"
#endif


// UART ISR: We use this ISR which just jumps to uart rx ISR in the bootloader's
// ISR table.
void  UART_RX_ISR(void) __attribute__((naked));
ISR(UART_RX_ISR)
{
    // Warning: this is dependent on where the bootloader is loaded in memory!
    // With 1024 words (2048 bytes) bootloader space, a given bootloader isr vector
    // is at address: 
    //
    //    boot_base_address + 4 * (isr_vector_number-1)
    //
    // XXX Note that datasheet starts vectors at 1, cf avr-gcc which starts them at 0
    // XXX Here we are using UART RX vector which is number 19 according to datasheet,
    // XXX and 18 according to avr-gcc
#if defined(ATMEGA328)
    // boot base is at 0x7800, add 72 (0x48) to give vector address of 0x7848
    // UART RX ISR is at vector 19, which is (19-1)*4=72 (0x48) bytes from start of memory
    asm("jmp 0x7848");
#elif defined(ATMEGA168)
    // boot base is at 0x3800, add 72 (0x48) to give vector address of 0x3848
    asm("jmp 0x3848");
#elif defined(ATMEGA8)
    // boot base is at 0x1800, add 72 (0x48) to give vector address of 0x1848
    asm("jmp 0x1848");
#else
  #error "This processor is not supported."
#endif
}

//! typedef for various function signature types
typedef void           (*PF_VOID)();
typedef char           (*PF_CHAR)();
typedef unsigned char  (*PF_UCHAR)();
typedef uint16_t       (*PF_UINT16)();

#define BOOT_FN_CALL(offset, msg)   return ((PF_UINT16)(( BOOT_FTAB_START  +offset )/2))(crc,data);

// Jump table offsets of the various functions.
// These must correspond to addresses as set in boot_ftab.S
BOOT_FADDR_XMODEM_UPDATE  0x0
BOOT_FADDR_UART_SET_BAUD  0x2
BOOT_FADDR_UART_INIT      0x4
BOOT_FADDR_UART_PUTC      0x6
BOOT_FADDR_UART_PUTS      0x8
BOOT_FADDR_UART_PUTS_P    0xa
BOOT_FADDR_UART_WRITE_P   0xc
BOOT_FADDR_UART_GETC      0xe
BOOT_FADDR_EEPROM_READ_BLOCK   0x10
BOOT_FADDR_EEPROM_UPDATE_BLOCK 0x12
BOOT_FADDR_MSG_INIT            0x14
BOOT_FADDR_MSG_RX_BYTE         0x16
BOOT_FADDR_MSG_SEND            0x18
BOOT_FADDR_MSG_TICK            0x1a

// function declarations
static __inline__ uint16_t bu_crc_xmodem_update(uint16_t crc, uint8_t data)
{
    BOOT_FN_CALL(BOOT_FADDR_XMODEM_UPDATE, "(crc,data)")
//  return ((PF_UINT16)(( BOOT_FTAB_START  +0 )/2))(crc,data);
}

__inline__ void uart_set_baud()
{
    BOOT_FN_CALL(BOOT_FADDR_UART_SET_BAUD, "()")
}

__inline__ void uart_init(char* buf, uint8_t buf_size)
{
    BOOT_FN_CALL(BOOT_FADDR_UART_INIT, "(buf, buf_size)")
}

__inline__ void uart_putc(const char c)
{
    BOOT_FN_CALL(BOOT_FADDR_UART_PUTC, "(c)")
}

__inline__ void uart_puts(const char* pc)
{
    BOOT_FN_CALL(BOOT_FADDR_UART_PUTS, "(c)")
}

__inline__ void uart_puts_P(const char* pc)
{
    BOOT_FN_CALL(BOOT_FADDR_UART_PUTS_P, "(pc)")
}

__inline__ void uart_write(char* buf, unsigned int size)
{
    BOOT_FN_CALL(BOOT_FADDR_UART_WRITE_P, "(buf,size)")
}

__inline__ char uart_getc()
{
    BOOT_FN_CALL(BOOT_FADDR_UART_GETC, "()")
}

__inline__ void bu_eeprom_read_block(void * __dst, const void *__src, size_t __n)
{
    BOOT_FN_CALL(BOOT_FADDR_EEPROM_READ_BLOCK, "(__dst, __src, __n)")
}

__inline__ void bu_eeprom_update_block (const void *__src, void *__dst, size_t __n)
{
    BOOT_FN_CALL(BOOT_FADDR_EEPROM_UPDATE_BLOCK, "(__dst, __src, __n)")
}

__inline__ void msg_init(msg_ctrl_t *msg_ctrl, uint8_t *buf, uint8_t buf_size, void (*handler)(msg_t *msg))
{
    BOOT_FN_CALL(BOOT_FADDR_MSG_INIT, "(msg_ctrl, buf, buf_size, handler)");
}

__inline__ void msg_rx_byte(msg_ctrl_t *msg_ctrl, uint8_t byte)
{
    BOOT_FN_CALL(BOOT_FADDR_MSG_RX_BYTE, "(msg_ctrl, byte)");
}

__inline__ void msg_send(uint8_t *msg_data, uint8_t len, void (*tx_byte_fn)(const char b))
{
    BOOT_FN_CALL(BOOT_FADDR_MSG_SEND, "(msg_data,len,tx_byte_fn)");
}

__inline__ void msg_tick(msg_ctrl_t *msg_ctrl)
{
    BOOT_FN_CALL(BOOT_FADDR_MSG_TICK, "(msg_ctrl)");
}
