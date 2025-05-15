bits 64

section .text
global gaussian_blur
; edi - высота исходного изображения
; esi - ширина исхожного изображения
; rdx - адрес буфера назначения
; rcx - адрес исходного буфера
gaussian_blur:
    push rbp
    mov rbp, rsp
    sub rsp, 32
; внутри функции:
; r8 - индекс строки, r9 - индекс столбца в итоговом буфере
; r11 - индекс в буфере назначения
; r10 - индекс в исходном буфере
    xor r8, r8
    mov [rbp - 8], rdx ; mul меняет значение rdx, сохраняем его на стеке
    mov [rbp - 16], r12; по соглашению надо сохранять
    mov [rbp - 24], r13
    r8_loop:
        ; r11 = r8 * wight + dest
        mov rax, r8
        mul rsi
        add rax, [rbp - 8]
        mov r11, rax 

        ; r10 = r8 * (wight + 2) + src
        
        mov rax, rsi
        add rax, 2
        mul r8
        add rax, rcx
        mov r10, rax
        

        xor r9, r9
        r9_loop:
            ; r12 - новое значение бита
            ; r13 - для временного хранения прибавляемого значение
            ; срединное значение с коэффициентом 4
            lea rax, [rsi + r10 + 2]
            movzx r12, byte [rax]
            sal r12, 1
            ; значения с коэффициентом 2
            lea rax, [r10 + 1]
            movzx r13, byte[rax]
            add r12, r13
            lea rax, [r10 + rsi + 2]
            movzx r13, byte[rax]
            add r12, r13
            add rax, 2
            movzx r13, byte[rax]
            add r12, r13
            lea rax, [r10 + rsi * 2 + 5]
            movzx r13, byte[rax]
            add r12, r13
            sal r12, 1
            ;значения с коэффициентом 1
            mov rax, r10
            movzx r13, byte[rax]
            add r12, r13
            add rax, 2
            movzx r13, byte[rax]
            add r12, r13
            lea rax, [r10 + rsi * 2 + 4]
            movzx r13, byte[rax]
            add r12, r13
            add rax, 2
            movzx r13, byte[rax]
            add r12, r13
            ;делим на 16
            shr r12, 4
            mov byte [r11], r12b
            inc r9
            inc r10
            inc r11
            cmp r9, rsi
            jb r9_loop
        inc r8
        cmp r8, rdi
        jb r8_loop
    mov r12, [rbp - 16]
    mov r13, [rbp - 24]
    leave
    ret

    


