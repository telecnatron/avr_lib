// -----------------------------------------------------------------------------
// Copyright 2018 Stephen Stebbing. telecnatron.com
//
//    Licensed under the Telecnatron License, Version 1.0 (the “License”);
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//        https://telecnatron.com/software/licenses/
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an “AS IS” BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.
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
#include "../mmp/mmp.h"

//! This is the address of the boot exported-function table,
//! and should be defined in Makefile
//! see boot_fstab.S for section definition.
// #define BOOT_FTAB_START 0x7FCE
#ifndef BOOT_FTAB_START
  #error "BOOT_FTAB_START must be defined to indicate starting address or the boot-function table"
#endif


// UART ISR: In the app, we use this ISR which just jumps to corresponding ISR in the bootloader's
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

// Jump table offsets of the various functions.
// These must correspond to addresses as set in boot_ftab.S
#define BOOT_FADDR_UART_SET_BAUD  0x0
#define BOOT_FADDR_UART_INIT      0x2
#define BOOT_FADDR_UART_PUTC      0x4
#define BOOT_FADDR_UART_PUTS      0x6
#define BOOT_FADDR_UART_PUTS_P    0x8
#define BOOT_FADDR_UART_WRITE     0xa
#define BOOT_FADDR_UART_GETC      0xc
#define BOOT_FADDR_MMP_INIT       0xe
#define BOOT_FADDR_MMP_RX_CH      0x10
#define BOOT_FADDR_MMP_SEND       0x12
#define BOOT_FADDR_MMP_TICK       0x14

// function declarations
__inline__ void uart_set_baud()
{
    ((PF_VOID)((BOOT_FTAB_START + BOOT_FADDR_UART_SET_BAUD)/2))();
}

__inline__ void uart_init(char* buf, uint8_t buf_size)
{
    ((PF_VOID)((BOOT_FTAB_START + BOOT_FADDR_UART_INIT)/2))(buf, buf_size);
}

__inline__ void uart_putc(const char c)
{
    ((PF_VOID)((BOOT_FTAB_START + BOOT_FADDR_UART_PUTC)/2))(c);
}

__inline__ void uart_puts(const char* pc)
{
    ((PF_VOID)((BOOT_FTAB_START + BOOT_FADDR_UART_PUTS)/2))(pc);
}

__inline__ void uart_puts_P(const char* pc)
{
    ((PF_VOID)((BOOT_FTAB_START + BOOT_FADDR_UART_PUTS_P)/2))(pc);
}

__inline__ void uart_write(char* buf, unsigned int size)
{
    ((PF_VOID)((BOOT_FTAB_START + BOOT_FADDR_UART_WRITE)/2))(buf, size);
}

__inline__ char uart_getc()
{
    return ((PF_CHAR)((BOOT_FTAB_START + BOOT_FADDR_UART_GETC)/2))();
}


__inline__ void mmp_init(mmp_ctrl_t *msg_ctrl,  uint8_t *buf, uint8_t buf_size,  void (*user_handler)(void *user_data, mmp_msg_t *msg), void *user_data)
{
    ((PF_VOID)((BOOT_FTAB_START + BOOT_FADDR_MMP_INIT)/2))(msg_ctrl, buf, buf_size, user_handler);
}

__inline__ void mmp_rx_ch(mmp_ctrl_t *mmp_ctrl, uint8_t ch)
{
    ((PF_VOID)((BOOT_FTAB_START + BOOT_FADDR_MMP_RX_CH)/2))(mmp_ctrl, ch);
}

__inline__ void mmp_send(uint8_t *msg_data, uint8_t len, uint8_t flags, void (*tx_byte_fn)(const char c))
{
    ((PF_VOID)((BOOT_FTAB_START + BOOT_FADDR_MMP_SEND)/2))(msg_data, len, flags, tx_byte_fn);
}

__inline__ void mmp_tick(mmp_ctrl_t *mmp_ctrl)
{
    ((PF_VOID)((BOOT_FTAB_START + BOOT_FADDR_MMP_TICK)/2))(mmp_ctrl);
}
