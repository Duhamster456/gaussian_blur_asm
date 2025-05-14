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
    sub rsp, 8
; внутри функции:
; r8 - индекс строки, r9 - индекс столбца в итоговом буфере
; rax - индекс в буфере назначения
; r10 - индекс в исходном буфере
    xor r8, r8
    r8_loop:
    ; rax = i * wight + dest
    mov rax, r8
    mul rax, rsi
    add rax, rdx

    ; r10 = i * (wight + 2) + src
    mov r10, rsi
    add r10, 2
    mul r10, r8
    add r10, rcx

    xor r9, r9
    r9_loop:
    inc rax
    inc r10

    mov r11, rsi
    add r11, 2
    mul r11, 

    leave
    ret

    


