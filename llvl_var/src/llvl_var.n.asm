section .data
    ; abs limits
    i32_limit_a_len dq  10
    i32_limit_a db "2147483647"
    i32_limit_h_len dq  8
    i32_limit_h db"7FFFFFFF"
    i32_limit_o_len dq  11
    i32_limit_o db "17777777777"

section .text


extern _var_atoi32
extern _var_atoi64
extern _var_atof32
extern _var_atof64


global _var_is_space_c_entry
_var_is_space_c_entry:
    mov al, cl
    jmp _is_space


; CALL / RET
; MODIFIES R14, RAX, R11, RDX
; EXPECTS NUMERIC TYPE TO BE IN [RSP + 96]
; EXPECTS R11 TO HOLD ASCII LEN
; EXPECTS R14 TO HOLD ASCII BEGIN
; RETURNS 1 IN AL IF ABS OF VALUE EXCEEDS I32 LIMIT
_exceeds_i32_limit:
    mov rax, r14
    inc rax
    mov rdx, R11
    dec rdx
    cmp byte [r14], '-'
    cmove r14, rax
    cmove r11, rdx


    cmp byte [RSP + 96], 'x'
    je  ._exceeds_i32_limit.as_hex

    cmp byte [RSP + 96], 'o'
    je  ._exceeds_i32_limit.as_octal

    ; DEFAULT
    ._exceeds_i32_limit.as_ascii:
        ; CHECK IF THE VALUE IS LESS THAN OR LONGER THAN THE LITERAL
        cmp r11, qword [rel i32_limit_a_len]
        jg .exceeds
        jl .short

        lea rax, [rel i32_limit_a]


        ; WRITE END TO R11
        .ascii.itr:
            cmp r11, 0
            je .short

            mov rdx, [rax]
            cmp [r14], dl
            jg .exceeds

            inc r14
            inc rax
            dec r11
            jmp .ascii.itr

        mov al, 0
        ret
    ; HEX
    ._exceeds_i32_limit.as_hex:
        ; CHECK IF THE VALUE IS LESS THAN OR LONGER THAN THE LITERAL
        cmp r11, qword [rel i32_limit_h_len]
        jg .exceeds
        jl .short

        lea rax, [rel i32_limit_h]
        
        .hex.itr:
            cmp r11, 0
            je .short

            mov rdx, [rax]
            cmp [r14], dl
            jg .exceeds

            inc r14
            inc rax
            dec r11
            jmp .hex.itr


    ; OCTAL
    ._exceeds_i32_limit.as_octal:
        ; CHECK IF THE VALUE IS LESS THAN OR LONGER THAN THE LITERAL
        cmp r11, qword [rel i32_limit_o_len]
        jg .exceeds
        jl .short

        lea rax, [rel i32_limit_o]

        .octal.itr:

           cmp r11, 0
            je .short

            mov rdx, [rax]
            cmp [r14], dl
            jg .exceeds

            inc r14
            inc rax
            dec r11
            jmp .octal.itr

.short:
    mov al, 0
    ret 
.exceeds:
    mov al, 1
    ret


    



; EXPECTS BYTE IN al
; RETURNS 0 OR 1 IN al
_is_space:
    cmp al, 32 ; SPACE
    je .true
    cmp al, 13 ; CARRIAGE RETURN
    je .true
    cmp al, 10 ; LINE FEED
    je .true
    cmp al, 9 ; HORZ. TAB
    je .true
    cmp al, 12 ; FORM FEED
    je .true
    cmp al, 11 ; VERT. TAB
    je .true
    
    mov al,0
    ret 
.true:
    mov al, 1
    ret



; MODIFIES RDX, R8
; EXPECTS R14 TO HOLD ASCII BEGIN
; RETURNS IN AL
; RETURNS THE NUMBER OF DIGITS IN R9
_is_floating_point:
    ; R14 IS THE CURSOR
    mov rdx, r14
    
    ; R8 holds the address end
    mov r8,rdx
    add r8,r11

    
.itr:
    cmp byte [rdx], '.'
    mov al,[rdx]
    je .true

    inc rdx
    cmp rdx, r8
    jl .itr

    mov al, 0
    ret
.true:
    mov al, 1
    ret



; a value is numeric if:
; it begins with 0o and is comprised only of digits 0-7  (octal)
; it begins with 0x and is comprised only of digits 0-16 (hexidecimal)
; it only contains numeric characters

; MODIFIES R8, R9, R10, RDX, RAX
; ASSUMES R14 TO HOLD VALUE, R11 TO HOLD VALUE LENGTH
; RETURN IS_NUMERIC IN AL
; RETURN VALUES:
; 0: not numeric
; 1: decimal
; 'o': octal
; 'x': hex
_is_numeric:
    sub rsp, 8
    mov byte [rsp], 0
    mov r8, r14
    add r8, r11
    ; R8 IS DATA END
    ; RDX IS THE CURSOR
    mov rdx, r14

    ; R9 IS SCRATCH REG.

    ; RAX IS SCRATCH REG.
    
    ; R10 HOLDS THE OCTAL/HEX STATE
    ; 0: presumed decimal
    ; 'o': presumed octal
    ; 'x': presumed hexadecimal
    mov r10, 0
    mov r9, 0

    ; check if it may be octal or hex
    mov al, [rdx]
    mov rax, rdx
    add rax, 1
    cmp byte [rdx], 48
    cmove rdx, rax ; inc rdx, non branching.
    
    ; check if data == end
    cmp rdx, r8
    je .false

    mov rax, 1
    cmp byte [rdx], 'o'
    cmove r10, [rdx]
    cmove r9, rax
    and r10, 0xFF ; TRIM R10

    cmp byte [rdx], 'o'
    je .skip_alpha_check

    ; check if it may be hexadecimal
    cmp byte [rdx], 'x'
    cmove r10, [rdx]
    cmove r9, rax
    and r10, 0xFF ; TRIM R10

    cmp byte [rdx], 'x'
    je .skip_alpha_check

    add rdx, r9
    
    
    ; check if data == end
    cmp rdx, r8
    je .false


.itr:
    ; if digit is not a numeric character; it's not numeric.
    
    mov al, [rdx]
    cmp al, '.'
    je .special_char_period

    cmp r10, 'x'
    je .handle_hex_alpha_check

    cmp al, '-'
    je .skip_alpha_check
    
    ; check if it's alpha
    cmp al, '0'
    jl .false
    cmp al, '9'
    jg .false

.skip_alpha_check:
.handle_hex_alpha_check.return:
.special_char_period.return:

    inc rdx
    cmp rdx, r8
    jl .itr

; true
    ; increment r14 (the value address) by 1 if it's octal / hex
    mov rax, 1
    cmp r10d, 0
    cmovne rax, r10
    add rsp, 8
    ret

.false:
    mov al, 0
    add rsp, 8
    ret

.special_char_period:
    cmp byte [rsp], 1
    je .false

    mov byte [rsp], 1
    jmp .special_char_period.return


.handle_hex_alpha_check:
    ; check 0-9
    cmp al, '0'
    jb .check_upper_hex                
    cmp al, '9'
    jbe .handle_hex_alpha_check.return

.check_upper_hex:
    ; check A-F
    cmp al, 'A'
    jb .check_lower_hex
    cmp al, 'F'
    jbe .handle_hex_alpha_check.return

.check_lower_hex:
    ; check a-f
    cmp al, 'a'
    jb .false
    cmp al, 'f'
    jbe .handle_hex_alpha_check.return

global _var_parse_arm64
_var_parse_arm64:
    ; check if the size of the data is 0, if so return
    cmp rdx, 0
    je .ret_success


    ; R9 HOLDS A POINTER TO THE WRITE HEADER

    sub rsp, 128 ; grow stack
    
    ; SAVE R12 R13 R14 R15 RBX
    mov [rsp+56], r12
    mov [rsp+64], r13
    mov [rsp+72], r14
    mov [rsp+80], rbx



    ;RSP + 32 = data_end
    mov rax, 0
    mov rax, rcx
    add rax, rdx

    mov [rsp+32], rax
    mov [rsp+40], r8 ; a ptr to the bytes written

    dec rax
    mov [rsp+48], rax ; data_end - 1

    add rsp, 24
    mov qword rdx, rsp  ; RDX HOLDS A POINTER TO THE LAST NODE IN THE LIST
                        ; BY DEFAULT, IT IS INITIALIZED TO AN 8-BYTE SCRATCH VALUE ON THE STACK: RSP+24
    sub rsp, 24
                
    ; R8 HOLDS THE NEEDED BYTE COUNT
    mov r8, 0


    ; r10 holds the number of bytes needed
    mov r10, 0

    ; r11 holds str len
    ; (which is length of the value string after final pass in pair)
    mov r11, 0
    
    mov r12, 0  ; R12 holds the key/value state
                ; for the current string.s
                ; 0: key
                ; 1: transient stage
                ; 2: value

    ; r13 holds the address of the key string
    mov r13, 0

    ; rbx holds the length of the key string
    mov rbx, 0

    ; r14 holds the address of the value string
    mov r14, 0

    

._main_itr_loop:

    cmp rcx, [rsp+48] ; last char
    je .handle_last_char


    mov al, [rcx];

    cmp al, ':'
    je .handle_colon

    cmp al, 13
    je .handle_string_break ; break on \n

    call _is_space
    
    cmp al, 1 ; is space
    je .handle_space

._main_itr_loop.ret_handle_colon_stage_II:
    ; beginning of mark
    cmp r11, 0
    je .mark


._main_itr_loop.mark_ret:
    ; inc str len
    inc r11


._main_itr_loop.continue:
    inc rcx ; inc cur ptr

    cmp rcx, [rsp+32] ; check cur against data_end
    jle ._main_itr_loop ; next loop itr.

.ret_success:
    mov rax, [rsp+40]
    mov [rax], r8 ; write to bytes needed ptr

    
    ; RESTORE R12 R13 R14 R15 RBX
    mov r12, [rsp+56]
    mov r13, [rsp+64]
    mov r14, [rsp+72]
    mov rbx, [rsp+80]


    add rsp, 128 ; shrink stack
    mov rax, 0
    ret
; END BODY _var_parse_arm64

.handle_last_char:
    ; inc str len
    inc r11
    jmp .handle_string_break


.handle_space:
    cmp r12, 2
    je ._main_itr_loop.mark_ret

    jmp .handle_string_break


.handle_colon:
    cmp r12, 2
    je ._main_itr_loop.ret_handle_colon_stage_II
    
    cmp r12, 0
    je .handle_string_break
    ; cmp r12, 1
    ; je ._main_itr_loop.continue

    ; r12 == 1
    mov r12, 1
    jmp ._main_itr_loop.continue


.mark:
    cmp r12, 0
    je .mark_key_begin

    cmp r12, 1
    je .mark_value_begin

.mark_key_begin:
    mov r13, rcx
    jmp ._main_itr_loop.mark_ret

.mark_value_begin: 
    mov al, [rcx]
    mov r11, 0 ; reset strlen
    mov r12, 2
    mov r14, rcx
    jmp ._main_itr_loop.mark_ret


.handle_string_break:

    ; check if the str len is greater than 0
    cmp r11, 0
    jg .if_is_string

    jmp ._main_itr_loop.continue

    .if_is_string:
        ; state transitions and value finalization
        cmp r12, 0
        je .if_is_string.key_to_value
        cmp r12, 2
        je .if_is_string.finalize

        ; 1
        jmp ._main_itr_loop.continue

        .if_is_string.key_to_value:
            mov rbx, r11 ; save strlen in rbx
            mov r14, rcx
            mov r11, 0 ; reset strlen
            mov r12, 1
            jmp ._main_itr_loop.continue
        .if_is_string.finalize:
            mov r12, 0
            jmp .finalize_pair

.finalize_pair:        



        ; SAVE R8, R9, R10, RDX
        mov [rsp+96], r8
        mov [rsp+104], r9
        mov [rsp+112], r10
        mov [rsp+120], rdx
        call _is_numeric

        ; RESTORE R8, R9, R10, RDX
        mov r8,  [rsp+96]
        mov r9,  [rsp+104]
        mov r10, [rsp+112]
        mov rdx, [rsp+120]
        
        cmp al, 0

        jne .finalize_number_pair

        jmp .finalize_string_pair       
    

.finalize_string_pair:
    add r8, 32 ;sizeof(VAR_NODE_STRING)
    cmp r9, 0 ; check if write header is NULL
    jne .finalize_string_pair.write

    ; complete finalize
    mov r11, 0 ; reset strlen
    jmp ._main_itr_loop.continue

.finalize_string_pair.write:


    mov word [r9], 5 ; type
    mov [r9+2], rbx ; nameLength
    mov [r9+8], r13 ; name
    mov [r9+16], r11 ; strlen
    mov [r9+24], r14 ; str

    cmp rdx, 0

    ; advance write header
    add r9, 32 ;sizeof(VAR_NODE_STRING)

    ; complete finalize
    mov r11, 0 ; reset strlen
    jmp ._main_itr_loop.continue



.finalize_number_pair:    
    ; rsp+96 holds the numeric type 1/'o'/'x'
    ; 2 = floating point
    mov [rsp+96], rax

    ; SAVE R8, RDX
    mov [rsp+112], r8
    mov [rsp+120], rdx
    call _is_floating_point
    ; RESTORE R8, RDX
    mov r8,  [rsp+112]
    mov rdx, [rsp+120]
    
    ; STORE FLOAT STATE IN RSP+88
    mov [rsp+88], rax


    cmp byte [rsp+88], 1
    je .finalize_as_float
    
; finalize as integer

    ; CALL / RET
    ; MODIFIES R14, RAX, R11, RDX
    ; EXPECTS NUMERIC TYPE TO BE IN [RSP + 96]
    ; EXPECTS R11 TO HOLD ASCII LEN
    ; EXPECTS R14 TO HOLD ASCII BEGIN
    
    mov [rsp+104], rdx
    mov [rsp+112], r11
    mov [rsp+120], r14

    call _exceeds_i32_limit
    mov rdx, [rsp+104]
    mov r11, [rsp+112]
    mov r14, [rsp+120]

    cmp al, 0
    je .finalize_as_i32

    jmp .finalize_as_i64

.finalize_as_float:
    cmp rax, 9
    jl .finalize_as_f32

    jmp .finalize_as_f64


.finalize_as_i32:
    add r8, 16 ;bytes written: inc by sizeof(VAR_NODE_I32)

    cmp r9, 0 ; check if write header is NULL
    jne .finalize_as_i32.write

    ; complete finalize
    mov r11, 0 ; reset strlen
    jmp ._main_itr_loop.continue


.finalize_as_i32.write:

    ; SAVE
    sub rsp, 32
    mov [rsp], rcx
    mov [rsp+8], r8
    mov [rsp+16], r9

    

    mov rcx, r14
    mov rdx, r11

    sub rsp, 32
    call _var_atoi32
    add rsp, 32

    ; RESTORE
    mov rcx, [rsp]
    mov r8,  [rsp+8]
    mov r9,  [rsp+16]
    add rsp, 32

    mov word [r9], 1 ; type
    mov [r9+2], rbx ; nameLength
    mov [r9+4], rax ; value
    mov [r9+8], r13 ; name
    

    ; advance write header
    add r9, 16 ;sizeof(VAR_NODE_I32)

    ; complete finalize
    mov r11, 0 ; reset strlen
    jmp ._main_itr_loop.continue

; finalize_as_i32 END





.finalize_as_i64:
    add r8, 24 ;bytes written: inc by sizeof(VAR_NODE_I64)

    cmp r9, 0 ; check if write header is NULL
    jne .finalize_as_i64.write

    ; complete finalize
    mov r11, 0 ; reset strlen
    jmp ._main_itr_loop.continue


.finalize_as_i64.write:

    sub rsp, 64
    mov [rsp], rcx
    mov [rsp+8], rdx
    mov [rsp+32], r8
    mov [rsp+40], r9

    

    mov rcx, r14
    mov rdx, r11

    sub rsp, 32
    call _var_atoi64
    add rsp, 32

    ; RESTORE
    mov rcx, [rsp]
    mov rdx, [rsp+8]
    mov r8,  [rsp+32]
    mov r9,  [rsp+40]
    add rsp, 64

    mov word [r9], 2 ; type
    mov [r9+2], rbx ; nameLength
    mov [r9+8], r13 ; name
    mov [r9+16], rax ; value


    ; advance write header
    add r9, 24 ;sizeof(VAR_NODE_I64)

    ; complete finalize
    mov r11, 0 ; reset strlen
    jmp ._main_itr_loop.continue

; finalize_as_i64 END











.finalize_as_f32:

    add r8, 16 ;bytes written: inc by sizeof(VAR_NODE_F32)

    cmp r9, 0 ; check if write header is NULL
    jne .finalize_as_f32.write
    ; complete finalize
    mov r11, 0 ; reset strlen
    jmp ._main_itr_loop.continue

.finalize_as_f32.write:

    ; SAVE
    sub rsp, 32
    mov [rsp], rcx
    mov [rsp+8], r8
    mov [rsp+16], r9

    

    mov rcx, r14
    mov rdx, r11

    sub rsp, 48
    call _var_atof32
    add rsp, 48

    ; RESTORE
    mov rcx, [rsp]
    mov r8,  [rsp+8]
    mov r9,  [rsp+16]
    add rsp, 32

    mov word [r9], 3; type
    mov      [r9+2], rbx  ; nameLength
    movss    [r9+4], xmm0 ; value
    mov      [r9+8], r13  ; name
    

    ; advance write header
    add r9, 16 ;sizeof(VAR_NODE_F32)

    ; complete finalize
    mov r11, 0 ; reset strlen
    jmp ._main_itr_loop.continue

; finalize_as_f32 END

.finalize_as_f64:
    add r8, 24 ;bytes written: inc by sizeof(VAR_NODE_F64)

    cmp r9, 0 ; check if write header is NULL
    jne .finalize_as_f64.write

    ; complete finalize
    mov r11, 0 ; reset strlen
    jmp ._main_itr_loop.continue


.finalize_as_f64.write:

    ; SAVE
    sub rsp, 32
    mov [rsp], rcx
    mov [rsp+8], r8
    mov [rsp+16], r9

    

    mov rcx, r14
    mov rdx, r11

    sub rsp, 48
    call _var_atof64
    add rsp, 48

    ; RESTORE
    mov rcx, [rsp]
    mov r8,  [rsp+8]
    mov r9,  [rsp+16]
    add rsp, 32

    mov word [r9], 4; type
    mov      [r9+2], rbx  ; nameLength
    movsd    [r9+8], xmm0 ; value
    mov      [r9+16], r13  ; name
    

    ; advance write header
    add r9, 24 ;sizeof(ENV_NODE_F64)

    ; complete finalize
    mov r11, 0 ; reset strlen
    jmp ._main_itr_loop.continue

; finalize_as_f64 END