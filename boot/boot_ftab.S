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
	;; uart functions
	rjmp uart_set_baud	; 0
	rjmp uart_init		; 2
	rjmp uart_putc		; 4
	rjmp uart_puts		; 6
	rjmp uart_puts_P	; 8
	rjmp uart_write		;10
	rjmp uart_getc	        ;12
        ;; message functions
	rjmp mmp_init		; 14
	rjmp mmp_rx_ch	        ; 16
	rjmp mmp_send		; 18
	rjmp mmp_tick		; 20



