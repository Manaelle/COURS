.global main
.text

main:
    mov r0, #8
    mov r1, #1
    add r2, r1, r0, lsr #2
    swi 0x123456

.data
