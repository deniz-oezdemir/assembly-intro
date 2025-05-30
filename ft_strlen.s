section .text     ; Code section (executable instructions go here)
global ft_strlen  ; Export function symbol for linker (make function visible to other files)

; C Prototype:
;     size_t strlen(const char *s);
;
; Description:
;     Calculates the length of a string by counting characters until null terminator.
;     This is a direct implementation of the standard C library function.
;
; Parameters (x86-64 calling convention):
;     rdi: Pointer to the input string (first argument is passed in rdi)
;
; Return:
;     rax: Number of bytes in the string (integer return values use rax)
;
; Notes:
;     - Does not handle NULL pointer input
;     - No bounds checking (expects proper null termination)
ft_strlen:                    ; Function entry point
    xor rax, rax              ; Zero out rax register (faster than mov rax, 0)
.loop:                        ; Label to mark start of loop (similar to while loop in C)
    cmp byte [rdi + rax], 0   ; Check if current byte is null
    je .end                   ; Jump to .end if equal, meaning we found null byte
    inc rax                   ; Increment length counter
    jmp .loop                 ; Unconditional jump back to .loop (continue iteration)
.end:                         ; Local label marking end of function
    ret                       ; Return to caller with length value in rax
                              ; No need to explicitly move value to rax as it's already there and caller knows to check rax
