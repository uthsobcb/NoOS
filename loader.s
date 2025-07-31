
global loader

extern kernel_main

MAGIC_NUMBER equ 0x1BADB002     
FLAGS        equ 0x0            
CHECKSUM     equ -MAGIC_NUMBER  

section .text
    align 4                     ; Align to 4-byte boundary (required by Multiboot)
    
    dd MAGIC_NUMBER             
    dd FLAGS                    
    dd CHECKSUM               

loader:
    mov esp, kernel_stack_top
    call kernel_main
    
    cli                         
.hang:
    hlt                         
    jmp .hang                   

section .bss
    align 4                     
    
    kernel_stack_bottom:
        resb 16384              ; Reserve (16KB)
    kernel_stack_top:           
    stack
