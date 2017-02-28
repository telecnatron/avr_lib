;;; -----------------------------------------------------------------------------
;;; Copyright Stephen Stebbing 2015. http://telecnatron.com/
;;;
;;; Licensed under the Apache License, Version 2.0 (the "License");
;;; you may not use this file except in compliance with the License.
;;; You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
;;;	
;;; Jump table for functions that are shared between bootloader and
;;; application. This is located at the top of flash memory.
;;; -----------------------------------------------------------------------------
;;; Name the section for the table. In Makefile we do:
;;;    LDFLAGS += -Wl,-section-start=.boot_ftab=$(BOOT_FTAB_START) -Wl,--undefined=_boot_ftab 
;;; to locate section at desired location.
;;;
;;; Note: the '"ax", @progbits' tells assembler that the section is
;;; allocatable, executable, and contains data.
.section .boot_ftab, "ax", @progbits

.global _boot_ftab

;; note that each entry takes 2 bytes
_boot_ftab:
	;; uart functions       ; byte offset
	rjmp uart_set_baud	; 0
	rjmp uart_init		; 2
	rjmp uart_putc		; 4
	rjmp uart_puts		; 6
	rjmp uart_puts_P	; 8
	rjmp uart_write		;10
	rjmp uart_getc	        ;12
        ;; mmp  message functions
	rjmp mmp_init		; 14
	rjmp mmp_rx_ch	        ; 16
	rjmp mmp_send		; 18
	rjmp mmp_tick		; 20



