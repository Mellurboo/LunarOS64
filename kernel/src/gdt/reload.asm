bits 64
global reload_segments
section .text

;reload segment registers in 64-bit mode
reload_segments:
    mov rax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, 0x10
    mov ss, ax

    ret
