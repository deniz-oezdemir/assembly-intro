section .text
global ft_strcmp

; C Prototype:
;     int strcmp(const char *s1, const char *s2);
;
; Description:
;     Compares two strings lexicographically and returns the difference.
;     This is a direct implementation of the standard C library function.
;
; Parameters (x86-64 calling convention):
;     rdi: Pointer to the first string (s1)
;     rsi: Pointer to the second string (s2)
;
; Return:
;     rax: The lexicographical difference between s1 and s2.
;
; Register usage:
;     rdi: First string pointer (parameter 1)
;     rsi: Second string pointer (parameter 2)
;     rax: Return value (difference between characters)
;     al:  Temporary storage for character comparison
;
; Notes:
;     - Does not handle NULL pointer input (undefined behavior)
;     - Uses cmpsb which compares bytes and auto-increments pointers
;     - Returns difference between first non-matching characters
ft_strcmp:                     ; Function entry point
.loop:                         ; Label marking the start of our comparison loop
  cmp BYTE [rdi], 0            ; Check if we've reached end of first string
  je .finish                   ; If so, finish the comparison
  cmpsb                        ; Compare string bytes at [rdi] and [rsi], then increment both
  je .loop                     ; If characters match, continue loop

  dec rdi                      ; Decrement rdi (back to differing character)
  dec rsi                      ; Decrement rsi (back to differing character)

.finish:                       ; Label for finishing the comparison
  xor rax, rax                 ; Zero out return register
  mov al, BYTE [rdi]           ; Load character from first string
  sub al, BYTE [rsi]           ; Subtract character from second string
  movsx eax, al                ; Sign-extend the 8-bit result to 32 bits
  ret                          ; Return with difference in rax
