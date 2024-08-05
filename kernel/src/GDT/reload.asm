bits 64

; reload.asm
global reload_segments

section .text

; Function to reload segment registers in 64-bit mode
reload_segments:
    ; In 64-bit mode, segment registers are not typically used for data access,
    ; but we can still show how you would set them up in a 32-bit compatibility context.

    ; Load the data segment selector (0x10) into segment registers
    ; Note: Segment operations are restricted in 64-bit mode
    mov rax, 0x10       ; Load the data segment selector into RAX
    mov ds, ax          ; Set DS register
    mov es, ax          ; Set ES register
    mov fs, ax          ; Set FS register
    mov gs, ax          ; Set GS register

    mov rax, 0x08       ; Load the data segment selector into RAX
    mov ds, ax          ; Set DS register
    mov es, ax          ; Set ES register
    mov fs, ax          ; Set FS register
    mov gs, ax          ; Set GS register

    ret
