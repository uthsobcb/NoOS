mov ah, 0x0e

mov al, 'N'
int 0x10
mov al, 'o'
int 0x10
mov al, 'O'
int 0x10
mov al, 'S'
int 0x10

jmp $
times 510-($-$$) db 0
dw 0xaa55