[bits 32]
[extern kernel_main]
[global _start]

_start:
    cli                 ; Ensure interrupts are disabled
    mov ebp, 0x90000    ; Set up stack base pointer
    mov esp, ebp        ; Set up stack pointer
    
    ; Write pink K to bottom-left (row 24, col 0)
    mov byte [0xB8000 + (24*80*2)], 'K'
    mov byte [0xB8000 + (24*80*2) + 1], 0x0D
    
    call kernel_main
    jmp $

