// -----------------------------------------------------------------------------
// Copyright Stephen Stebbing 2015. http://telecnatron.com/
// $Id: boot_functions.c 403 2015-12-27 03:11:36Z steves $
// -----------------------------------------------------------------------------
/**
 * @file   boot_functions.h
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
#include "../msg/msg.h"

//! This is the address of the boot exported-function table,
//! and should be defined in Makefile
//! see boot_fstab.S for section definition.
//! #define BOOT_FTAB_START 0x7FCE
#ifndef BOOT_FTAB_START
  #error "BOOT_FTAB_START must be defined to indicate starting address or the boot-function table"
#endif


// UART ISR: We use this ISR which just jumps to uart rx ISR in the bootloader's
// ISR table.
void  UART_RX_ISR(void) __attribute__((naked));
ISR(UART_RX_ISR)
{
    // Warning: this is dependent on where the bootloader is loader in memory!
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


// offset 0
static __inline__ uint16_t bu_crc_xmodem_update(uint16_t crc, uint8_t data)
{
  return ((PF_UINT16)(( BOOT_FTAB_START  +0 )/2))(crc,data);
}

// offset 2
__inline__ void uart_set_baud()
{
    ((PF_VOID)((BOOT_FTAB_START + 0x2)/2))();
}

// offset 4
__inline__ void uart_init(char* buf, uint8_t buf_size)
{
    ((PF_VOID)((BOOT_FTAB_START + 0x4)/2))(buf,buf_size);
}

// offset 6
__inline__ void uart_putc(const char c)
{
    ((PF_VOID)((BOOT_FTAB_START + 0x6)/2))(c);
}

// offset 8
__inline__ void uart_puts(const char* pc)
{
    ((PF_VOID)((BOOT_FTAB_START + 0x8)/2))(pc);
}

// offset 10
__inline__ void uart_puts_P(const char* pc)
{
    ((PF_VOID)((BOOT_FTAB_START + 0xA)/2))(pc);
}

// offset 12
__inline__ void uart_write(char* buf, unsigned int size)
{
    ((PF_VOID)((BOOT_FTAB_START + 0xC)/2))(buf,size);
}

// offset 14
__inline__ char uart_getc()
{
    return ((PF_CHAR)((BOOT_FTAB_START + 0xE)/2))();
}

// offset 16
__inline__ void bu_eeprom_read_block(void * __dst, const void *__src, size_t __n)
{
    ((PF_VOID)((BOOT_FTAB_START + 0x10)/2))(__dst, __src, __n);
}

// offset 18
__inline__ void bu_eeprom_update_block (const void *__src, void *__dst, size_t __n)
{
    ((PF_VOID)((BOOT_FTAB_START + 0x12)/2))(__dst, __src, __n);
}

// offset 20
__inline__ void msg_init(msg_ctrl_t *msg_ctrl, uint8_t *buf, uint8_t buf_size, void (*handler)(msg_t *msg))
{
    ((PF_VOID)((BOOT_FTAB_START + 0x14)/2))(msg_ctrl, buf, buf_size, handler);
}

//! offset 22
__inline__ void msg_rx_byte(msg_ctrl_t *msg_ctrl, uint8_t byte)
{
    ((PF_VOID)((BOOT_FTAB_START + 0x16)/2))(msg_ctrl, byte);
}

//! offset 24
__inline__ void msg_send(uint8_t *msg_data, uint8_t len, void (*tx_byte_fn)(const char b))
{
    ((PF_VOID)((BOOT_FTAB_START + 0x18)/2))(msg_data,len,tx_byte_fn);
}

//! offset 26
__inline__ void msg_tick(msg_ctrl_t *msg_ctrl)
{
    ((PF_VOID)((BOOT_FTAB_START + 0x1a)/2))(msg_ctrl);
}

