.global sym3
.extern sym5, sym8, sym9, sym4

.section code
sym1:
    .word sym1, 12345, sym12, 0x12345, 54321, sym14, sym2
sym3:
    ld $1000, %r1
    beq %r4, %r10, 0x2000 # this is comment
sym12:
    xor %r2, %pc
    bgt %r15, %sp, 1000
sym14:
    jmp sym1
    push %r1
sym10:
    ld [%sp], %r12
    pop %r1
.section code2
    bne %r11, %r2, sym3
    csrwr %r2, %status
sym11:
    .skip 10
    csrrd %cause, %r1
    csrrd %handler, %r3
    call 10000
.section code3
    sub %r12, %r2
    mul %r5, %r6
sym2:
sym15:
    st %sp, [%sp + 1234]
    div %r13, %r12
    .ascii "this is string"
    halt
.end