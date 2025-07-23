section .data
    three dq 3.
    two dq 2.
    one dq 1.
section .text
global f1, f2, f3

f1:
    push ebp
    mov ebp, esp
    finit
    fld qword [ebp + 8]
    fchs

    fldl2e
    fmulp
    fld st0
    frndint
    fsub st1, st0
    fxch

    f2xm1
    fld1
    faddp st1, st0
    fscale
    fstp st1

    push 3
    fild dword [esp]
    add esp, 4
    faddp

    leave
    ret

f2:
        push ebp
        mov ebp, esp
        finit

        fld qword[ebp + 8]
        fmul qword[two]
        push 2
        fisub dword[esp]
        add esp, 4
        leave
        ret

f3:
    push ebp
    mov ebp, esp
    finit

    push 1
    fild dword[esp]
    add esp, 4
    fld qword[ebp + 8]
    fdivp

    leave
    ret
