[bits 32]
[extern kernel_main]

MULTIBOOT_HEADER_MAGIC  equ 0x1BADB002
MULTIBOOT_HEADER_FLAGS  equ 0
CHECKSUM                equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

section .multiboot
align 4
dd MULTIBOOT_HEADER_MAGIC
dd MULTIBOOT_HEADER_FLAGS
dd CHECKSUM

section .text
global loader
loader:
    ; ------------------------
    ; Task 1: Set EAX to CAFEBABE
    ; ------------------------
    mov eax, 0xCAFEBABE

    ; ------------------------
    ; Task 2: Setup stack & call C kernel
    ; ------------------------
    mov esp, stack_space + stack_size
    push eax          ; pass CAFEBABE to C (optional)
    call kernel_main

hang:
    jmp hang

section .bss
align 16
stack_size equ 16384
stack_space:
    resb stack_size
