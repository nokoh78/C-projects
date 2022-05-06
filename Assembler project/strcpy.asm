bits 64
global main
default rel
extern printf
extern scanf

section .data
    input_format db '%s' , 0
    output_format db '%s' , 10, 0
section .bss
    input resb 1024
    output resb 1024
section .text
    main:
        sub rsp, 8
        lea rsi, [input]
        lea rdi, [input_format]
        mov al , 0
        call scanf wrt ..plt

        lea    rsi, [input]
        lea    rdi, [output]

        loop:
        mov    al, [rsi]
        movsb
        cmp    al, 0
        je    print
        jmp    loop

        
        print:
        lea rsi, [output]
        lea rdi, [output_format]
        mov al, 0
        call printf wrt ..plt



        add rsp, 8
        sub rax, rax
        ret