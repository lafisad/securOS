[bits 32]
[org 0x1000]

start:
    ; Print "securOS Kernel Loaded" to video memory
    mov esi, msg_kernel
    mov edi, 0xb8000    ; VGA text buffer
    
.loop:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0f        ; White on black attribute
    mov [edi], ax
    add edi, 2
    jmp .loop

.done:
    jmp $               ; Infinite loop

msg_kernel db "securOS Kernel Loaded", 0
