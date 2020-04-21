
        global  set_bit_elem
        global  get_bit_elem
        section .text

set_bit_elem:
        push rbp            ; save the base pointer on the stack (at rsp+8)
        mov rbp, rsp        ; set up the rbp for the bottom of this frame

        ; rdi contains array pointer
        ; rsi contains row width
        ; rdx contains row
        ; rcx contains col

        ; add your code here
        mov rax, rdx
        imul rax, rsi
        add rax, rcx

        ; Copying our index for later use
        mov r9, rax
        cmp rax, 8
        jle .byteskip

; Where we will be dividing our index by 8. Will keep dividing until we get a remainder which represents the byte offset
.indiv:
        sub rax, 8
        cmp rax, 8
        jge .indiv
        
.byteskip:
        ; This is where rax now has the value of byte_offset
        mov r15, rax

        mov r11, 1
        cmp r15, 0
        je .flip

.bitmask:
        shl r11, 1
        dec r15
        cmp r15, 0
        jne .bitmask

.flip:
        add rdi, r9
        or [rdi], r11

        mov rsp, rbp        ; restore stack pointer to before we pushed parameters onto the stack
        pop rbp             ; remove rbp from the stack to restore rsp to initial value
        ret                ; return value in rax


get_bit_elem:
        push rbp            ; save the base pointer on the stack (at rsp+8)
        mov rbp, rsp        ; set up the rbp for the bottom of this frame

        ; rdi contains array pointer
        ; rsi contains row width
        ; rdx contains row
        ; rcx contains col
        mov rax,rdx
        imul rax,rsi
        add rax,rcx


        mov r9, rax
        cmp rax, 8
        jle .byteskip

; Where we will be dividing our index by 8. Will keep dividing until we get a remainder which represents the byte offset
.indiv:
        sub rax, 8
        cmp rax, 8

        jge .indiv

; If we do not need to skip by the byte offset, .byteskip will keep going without it.
.byteskip:
        mov r15, rax
        mov r11, 1
        cmp r15, 0

        je .flip

; Will keep shifting by 1 until our bit offset value is zero.
.bitmask:
        shl r11, 1
        dec r15
        cmp r15, 0
        jne .bitmask

; Takes our value and flips the bit at the designated array index
.flip:
        add rdi, r9
        and [rdi], r11
        mov rax, [rdi]
        cmp rax, 0
        je .L3
        mov rax, 1


        
.restore:
        mov rsp, rbp        ; restore stack pointer to before we pushed parameters onto the stack
        pop rbp             ; remove rbp from the stack to restore rsp to initial value
        ret                 ; return value in rax

.L3:
        mov rax, 0
        jmp .restore
