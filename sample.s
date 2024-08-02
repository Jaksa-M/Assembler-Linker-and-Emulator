.global a, c
.extern b
.section text
    .word 12345
    beq %r1, %r2, a
    beq %r0, %r0, e
    beq %r0, %r0, b
    beq %r0, %r0, d
    call d
.ascii "Hey mama"
d:
    ld b, %r0
    ld c, %r1
    ld e, %r2
.section data
e:
    ld a, %r1
    csrwr %r1, %handler
a:
    ld e, %sp
    ld c, %r1
c:
    csrwr %r1, %handler
.end