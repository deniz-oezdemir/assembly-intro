section .text
global ft_strdup

; C Prototype:
;    char *strdup(const char *s);
;
; Description:
;    Duplicates a string into newly allocated memory.
;
; Parameters:
;    rdi: Pointer to the source string
;
; Return:
;    rax: Pointer to the newly allocated duplicate string
;         (NULL if memory allocation fails)

extern ft_strlen
extern ft_strcpy
extern malloc

ft_strdup:
    push rdi                  ; Store original string pointer on the stack so we can use it later
    call ft_strlen            ; Calculate the length of our input string (result in rax)
    inc rax                   ; Add 1 byte for the null terminator ('\0') at the end
    mov rdi, rax              ; Move the total size needed to rdi (first argument for malloc)
    call malloc wrt ..plt     ; Request memory allocation (result pointer in rax)
    test rax, rax             ; Check if malloc succeeded by testing if rax is zero
    jz .error                 ; If allocation failed (rax is zero), jump to error handling

    mov rdi, rax              ; Set first argument for strcpy (destination = new memory)
    pop rsi                   ; Get original string pointer from stack into second argument
    call ft_strcpy            ; Copy string from source (rsi) to destination (rdi)
    ret                       ; Return with result in rax (pointer to the new string)

.error:
    pop rdi                   ; Remove saved pointer from stack to prevent memory leak
    ret                       ; Return with rax=0 (NULL) to indicate failure
