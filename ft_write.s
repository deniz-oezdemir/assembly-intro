section .text
global ft_write

; C Prototype:
;    ssize_t write(int fd, const void *buf, size_t count);
;
; Parameters (x86-64 calling convention):
;    rdi: File descriptor
;    rsi: Pointer to buffer with data
;    rdx: Number of bytes to write
;
; Return:
;    rax: Number of bytes written (>= 0), or -1 on error

extern __errno_location        ; Tells the assembler that __errno_location is defined elsewhere
                               ; This function returns the address of the thread-local errno variable
                               ; We'll call it to properly set errno when system call fails

SYS_WRITE equ 1                ; Define constant 1 as the Linux syscall number for write

ft_write:
    mov rax, SYS_WRITE         ; Put the write syscall number (1) into rax
    syscall                    ; Call the kernel to execute write operation
    cmp rax, 0                 ; Check if result is negative (error occurred)
    jl .error_handle           ; If negative, jump to error handling code
    ret                        ; Otherwise, return normally with bytes written count in rax

.error_handle:
    neg rax                           ; Make error code positive (Linux returns negative error codes)
    mov rdi, rax                      ; Copy error code to rdi as first parameter for next function call
    call __errno_location wrt ..plt   ; Call C library function to get pointer to errno variable
                                      ; "wrt ..plt" means "with respect to Procedure Linkage Table"
                                      ; This is needed for position-independent code in dynamic linking
    mov [rax], rdi                    ; Store our error code at the memory location (errno) pointed to by rax
    mov rax, -1                       ; Set return value to -1 (standard error return value in C)
    ret                               ; Return to caller with rax = -1
