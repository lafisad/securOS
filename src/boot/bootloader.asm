[org 0x7c00]
[bits 16]

KERNEL_OFFSET equ 0x10000

start:
    ; Save boot drive
    mov [BOOT_DRIVE], dl
    
    ; Set up segment registers
    xor ax, ax
    mov ds, ax
    mov ss, ax
    mov sp, 0xFFFF

    ; Print welcome message
    mov si, msg_real_mode
    call print_string

    ; Load Kernel
    call load_kernel

    ; Switch to Protected Mode
    call switch_to_pm

    jmp $

; ------------------------------------------------------------------
; 16-bit Routines
; ------------------------------------------------------------------
print_string:
    push ax
    push si
.loop:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0e
    int 0x10
    jmp .loop
.done:
    pop si
    pop ax
    ret

load_kernel:
    mov si, msg_load_kernel
    call print_string

    ; Set up ES:BX to load to 0x1000:0000 (Physical 0x10000)
    mov ax, 0x1000
    mov es, ax
    xor bx, bx              ; Destination offset 0
    
    mov dl, [BOOT_DRIVE]    ; Boot drive
    
    ; Start reading from:
    mov ch, 0x00            ; Cylinder 0
    mov dh, 0x00            ; Head 0
    mov cl, 0x02            ; Sector 2
    
    mov bp, 0               ; Counter for sectors read

read_sector_loop:
    cmp bp, 60              ; Read 60 sectors (30KB)
    je read_done

    call read_sector_retry  ; Read one sector with retries
    
    add bx, 512             ; Advance buffer
    inc bp                  ; Increment counter
    
    ; Update Geometry
    inc cl                  ; Next sector
    cmp cl, 19              ; Sector > 18?
    jl read_sector_loop
    
    ; Next Track/Head logic
    mov cl, 0x01            ; Reset sector to 1
    inc dh                  ; Next head
    cmp dh, 0x02            ; Head > 1?
    jl read_sector_loop
    
    mov dh, 0x00            ; Reset head to 0
    inc ch                  ; Next cylinder
    jmp read_sector_loop

read_done:
    ret

; Function to read one sector with retries
read_sector_retry:
    mov di, 0               ; Retry counter

.retry:
    mov ah, 0x02            ; BIOS read sector
    mov al, 0x01            ; Read 1 sector
    
    push cx                 ; Save CX (Cylinder/Sector)
    push dx                 ; Save DX (Head/Drive)
    
    int 0x13
    
    pop dx                  ; Restore DX
    pop cx                  ; Restore CX
    
    jnc .success            ; If no error, we are good

    ; Error: Reset disk and retry
    push ax                 ; Save Error Code
    xor ah, ah
    push dx
    int 0x13
    pop dx
    pop ax                  ; Restore Error Code
    
    inc di
    cmp di, 3               ; Try 3 times
    jl .retry
    
    jmp disk_error          ; Failed after 3 retries

.success:
    ret

disk_error:
    mov si, msg_disk_error
    call print_string
    jmp $

; ------------------------------------------------------------------
; GDT
; ------------------------------------------------------------------
gdt_start:

gdt_null:
    dd 0x0
    dd 0x0

gdt_code:
    dw 0xffff       ; Limit (bits 0-15)
    dw 0x0          ; Base (bits 0-15)
    db 0x0          ; Base (bits 16-23)
    db 10011010b    ; 1st flags, type flags
    db 11001111b    ; 2nd flags, Limit (bits 16-19)
    db 0x0          ; Base (bits 24-31)

gdt_data:
    dw 0xffff       ; Limit (bits 0-15)
    dw 0x0          ; Base (bits 0-15)
    db 0x0          ; Base (bits 16-23)
    db 10010010b    ; 1st flags, type flags
    db 11001111b    ; 2nd flags, Limit (bits 16-19)
    db 0x0          ; Base (bits 24-31)

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

; ------------------------------------------------------------------
; Switch to Protected Mode
; ------------------------------------------------------------------
switch_to_pm:
    cli                     ; Disable interrupts
    lgdt [gdt_descriptor]   ; Load GDT
    
    mov eax, cr0
    or eax, 0x1             ; Set PE (Protection Enable) bit
    mov cr0, eax
    
    jmp CODE_SEG:init_pm    ; Far jump to flush pipeline

[bits 32]
init_pm:
    mov ax, DATA_SEG        ; Update segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    ; Debug: Write 'P' to top-right to confirm we're in protected mode
    mov byte [0xB8000 + (79*2)], 'P'
    mov byte [0xB8000 + (79*2) + 1], 0x0B    ; Light cyan on black
    
    mov ebp, 0x90000        ; Update stack position
    mov esp, ebp
    
    call KERNEL_OFFSET      ; Jump to kernel
    
    jmp $

; ------------------------------------------------------------------
; Data
; ------------------------------------------------------------------
BOOT_DRIVE      db 0
msg_real_mode   db "Started in 16-bit Real Mode", 0x0D, 0x0A, 0
msg_load_kernel db "Loading kernel...", 0x0D, 0x0A, 0
msg_disk_error  db "Disk read error!", 0

; ------------------------------------------------------------------
; Padding and Magic
; ------------------------------------------------------------------
times 510-($-$$) db 0
dw 0xaa55
