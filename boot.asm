[org 0x7c00]
[bits 16]

start:
    ; print real mode message
    mov si, real_msg
    call print_string

    ; enable A20 line
    call enable_a20

    ; load GDT
    cli
    lgdt [gdt_descriptor]

    ; switch to protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; far jump to flush pipeline
    jmp CODE_SEG:protected_mode

; ------------------------------------------
; 16-bit functions
; ------------------------------------------
print_string:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0e
    int 0x10
    jmp print_string
.done:
    ret

enable_a20:
    in al, 0x92
    or al, 2
    out 0x92, al
    ret

real_msg db 'Real Mode: CDFS-OS', 13, 10, 0

; ------------------------------------------
; GDT (Global Descriptor Table)
; ------------------------------------------
gdt_start:
    dq 0x0                          ; null descriptor

gdt_code:
    dw 0xffff                        ; limit low
    dw 0x0                            ; base low
    db 0x0                            ; base middle
    db 10011010b                      ; access (present, ring0, code, readable)
    db 11001111b                      ; granularity (4k, 32-bit)
    db 0x0                            ; base high

gdt_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b                      ; access (data, writable)
    db 11001111b
    db 0x0
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

; ------------------------------------------
; 32-bit Protected Mode
; ------------------------------------------
[bits 32]
protected_mode:
    ; set up segment registers
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; set up stack
    mov ebp, 0x90000
    mov esp, ebp

    ; print protected mode message
    mov esi, prot_msg
    call print_string_pm

    ; hang forever
    jmp $

print_string_pm:
    pusha
    mov edx, 0xb8000                  ; VGA text buffer
.loop:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0f                       ; white on black
    mov [edx], ax
    add edx, 2
    jmp .loop
.done:
    popa
    ret

prot_msg db 'Protected Mode: CDFS-OS Ready!', 0

times 510-($-$$) db 0
dw 0xaa55