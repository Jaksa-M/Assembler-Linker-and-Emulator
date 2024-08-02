.global sym5, sym8, sym9, sym4
.extern sym3

.section code
    ld $sym3, %sp
sym5:
    .word sym5, 12345, sym2, 0x12345, 54321, sym4, sym2
sym8:
    ld $1000, %r1
    beq %r4, %r10, 0x2000 # this is comment
sym2:
    xor %r2, %pc
    bgt %r15, %sp, 1000
sym4:
    jmp sym5
    push %r1
sym16:
    ld [%sp], %r12
    pop %r1
    xchg %r13, %r2
    add %r13, %r2
.section code2
    bne %r11, %r2, sym3
    csrwr %r2, %status
sym9:
    .skip 10
    csrrd %cause, %r1
    csrrd %handler, %r3
    call 10000
    sub %r12, %r2
    mul %r5, %r6
    st %sp, [%sp + 1234]
    div %r13, %r12
    .ascii "this is string"
    not %r2
    and %r2, %r9
    or %r5, %r2
    xor %r5, %r8
    shl %r14, %r4
    shr %r13, %r2
    ret
    iret
    int
    halt
.end