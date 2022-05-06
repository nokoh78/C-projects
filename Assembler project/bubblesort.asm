bits    64
global  main
default rel
extern  printf
extern  scanf

section .data
    input_format    db '%d',0
    output_format   db '%d ' ,0
    output_end      db  0xA ,   0
section .bss
    array   resd    100
section .text
    main:
        sub rsp,    8
        mov r12,    0
        lea rbx,    [array]
    input_loop:

        lea rsi,    [rbx + r12 * 4]
        lea rdi,    [input_format]
        mov al ,    0
        call scanf wrt ..plt
        inc r12

        cmp rax ,   1
        je  input_loop
    dec r12
    ; r12 to jest n czyli liczba elementow - 1
    ; r11 to jest j 
    ; r10 to jest i

    mov r10, 0
    ;sort
    dec r12
    loop_out:
        mov r11, r12
    loop_in:
        lea rax, [array]
        mov ebx, [rax + r11*4]
        mov edx, [rax + r11*4 - 4]
        cmp ebx, edx
        jg loop_in_end
        mov [rax + r11*4], edx
        mov [rax + r11*4 - 4], ebx
    loop_in_end:
        dec r11
        cmp r11, r10
        jg loop_in

    loop_out_end:
        inc r10
        cmp r12, r10
        jne loop_out

    





    
    lea rbx,    [array]
    mov r15, 0
    inc r12
    
    output_loop:
        
        mov rsi, [rbx + r15*4]
        lea rdi, [output_format]
        mov al , 0
        call printf wrt ..plt  
        inc r15
        cmp r12,    r15
        jne output_loop

    lea rdi, [output_end]
    call printf wrt ..plt
    add rsp, 8
    sub rax, rax
    ret