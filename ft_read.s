section .text
global ft_read

; C Prototype:
;    ssize_t read(int fd, void *buf, size_t count);
;
; Description:
;    Reads data from a file descriptor into a buffer.
;    This is a direct implementation of the standard C library function.
;
; Parameters (x86-64 calling convention):
;    rdi: File descriptor to read from
;    rsi: Pointer to the buffer to read into
;    rdx: Number of bytes to read
;
; Return:
;    rax: Number of bytes read (>= 0), or -1 on error
;
; Register usage:
;    rdi: File descriptor (parameter 1)
;    rsi: Buffer pointer (parameter 2)
;    rdx: Byte count (parameter 3)
;    rax: Syscall number / Return value
;
; Notes:
;    - Does not handle NULL buffer (undefined behavior)
;    - Uses Linux syscall interface

extern __errno_location        ; Tells the assembler that __errno_location is defined elsewhere

SYS_READ equ 0                 ; Define constant 0 as the Linux syscall number for read

ft_read:                       ; Function entry point
    mov rax, SYS_READ          ; Set syscall number (0 for read)
    syscall                    ; Call the kernel to execute read operation
    cmp rax, 0                 ; Check if result is negative (error occurred)
    jl .error_handle           ; If negative, jump to error handling code
    ret                        ; Otherwise, return normally with bytes read count in rax

.error_handle:                        ; Label for error handling
    neg rax                           ; Make error code positive (Linux returns negative error codes)
    mov rdi, rax                      ; Copy error code to rdi as first parameter for next function call
    call __errno_location wrt ..plt   ; Call C library function to get pointer to errno variable
    mov [rax], rdi                    ; Store our error code at the memory location (errno) pointed to by rax
    mov rax, -1                       ; Set return value to -1 (standard error return value in C)
    ret                               ; Return to caller with rax = -1
