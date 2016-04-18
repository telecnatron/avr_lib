;;; -----------------------------------------------------------------------------
;;; Copyright Stephen Stebbing 2015. http:;;;telecnatron.com/
;;; $Id: $
;;;	
;;; Jump table for functions that are shared between bootloader and
;;; application
;;; -----------------------------------------------------------------------------
;;; Note: the '"ax", @progbits' tells assembler that the section is
;;; allocatable, executable, and contains data.
.section .boot_ftab, "ax", @progbits

.global _boot_ftab

;; note that each entry takes 2 bytes
_boot_ftab:
	;;  crc functions
	rjmp bu_crc_xmodem_update	; 0
	;; uart functions
	rjmp uart_set_baud	; 2
	rjmp uart_init		; 4
	rjmp uart_putc		; 6
	rjmp uart_puts		; 8
	rjmp uart_puts_P	;10
	rjmp uart_write		;12
	rjmp uart_getc	        ;14
	;; eeprom functions
	rjmp bu_eeprom_read_block    ; 16
	rjmp bu_eeprom_update_block  ; 18
        ;; message functions
	rjmp msg_init		; 20
	rjmp msg_rx_byte	; 22
	rjmp msg_send		; 24
	rjmp msg_tick		; 26



