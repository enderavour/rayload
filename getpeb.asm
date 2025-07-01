section .text


global getkernel32base
global scmp
global exit

getkernel32base:
    mov rax, gs:[0x60]
    mov rax, [rax + 0x18]
    mov rax, [rax + 0x20]
    mov rax, [rax]
    mov rax, [rax]
    sub rax, 0x10
    mov rax, [rax + 0x30]
    ret

scmp:
    mov rsi, rcx
    mov rdi, rdx
    mov rcx, r8
    cld
    repe cmpsb
    jne .neq
    xor rax, rax
    ret

.neq:
    mov rax, 1
    ret

exit:
    xor r10, r10
    xor rcx, rcx
    mov rax, 0x2C
    syscall
    ret