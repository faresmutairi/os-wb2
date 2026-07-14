BITS 32

MULTIBOOT_MAGIC    equ 0x1BADB002
MULTIBOOT_FLAGS    equ 0x00000003
MULTIBOOT_CHECKSUM equ -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)
TEST_MARKER        equ 0xCAFEBABE
STACK_SIZE         equ 16384

section .multiboot
align 4
    dd MULTIBOOT_MAGIC
    dd MULTIBOOT_FLAGS
    dd MULTIBOOT_CHECKSUM

section .text
global loader
extern kernel_main

loader:
    ; Task 1: place the known test value in EAX exactly as required.
    mov eax, TEST_MARKER

    ; Task 2: create a private 16 KiB stack aligned to 16 bytes.
    mov esp, stack_top
    and esp, 0xFFFFFFF0

    ; Keep the stack 16-byte aligned immediately before CALL and pass the
    ; EAX test value to kernel_main using the 32-bit cdecl convention.
    sub esp, 12
    push eax
    call kernel_main
    add esp, 16

.halt:
    cli
    hlt
    jmp .halt

section .bss
align 16
stack_bottom:
    resb STACK_SIZE
stack_top:
section .note.GNU-stack noalloc noexec nowrite progbits
