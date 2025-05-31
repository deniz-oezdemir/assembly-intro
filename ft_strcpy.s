section .text
global ft_strcpy

; C Prototype:
;     char *strcpy(char *dest, const char *src);
;
; Description:
;     Copies a string from source to destination, including the null terminator.
;     This is a direct implementation of the standard C library function.
;
; Parameters (x86-64 calling convention):
;     rdi: Pointer to the destination buffer (first argument)
;     rsi: Pointer to the source string (second argument)
;
; Return:
;     rax: Pointer to the destination string (original dest value)
;
; Register usage:
;     rdi: Destination string pointer (parameter 1)
;     rsi: Source string pointer (parameter 2)
;     rax: Return value (original destination pointer)
;     ecx: Byte counter/offset
;     dl:  Temporary storage for current byte
; Notes:
;     - Does not handle NULL pointer input (undefined behavior)
;     - No bounds checking (caller must ensure dest has enough space)
;     - Assumes source and destination don't overlap
;     - Will copy the null terminator ('\0') to mark the end of string
ft_strcpy:                     ; Function entry point
  mov rax, rdi                 ; Save destination pointer in rax for return value
  xor ecx, ecx                 ; Zero out ecx register (counter) - using 32-bit version to save space
.loop:                         ; Label marking the start of our copy loop
  mov dl, byte [rsi + rcx]     ; Load a byte from source string at offset rcx into dl register (8-bit)
  mov byte [rdi + rcx], dl     ; Store that same byte to destination at offset rcx
  inc rcx                      ; Increment our position counter
  test dl, dl                  ; Check if the byte we just copied is zero (end of string)
  jnz .loop                    ; Jump if not zero (jnz) back to .loop and continue copying
  ret                          ; Return to caller with rax holding the destination pointer
