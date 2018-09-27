.global main
.text

main:

    mov r0, #11
    mov r1, #5
    subs r0, r1
    sbc r0, r0 ,r1

    mov r0, #4
    mov r1, #5
    subs r0, r1
    sbc r0, r0 ,r1

    mov r0, #5
    mov r1, #11
    subs r0, r1
    rsb r0, r0 ,r1

    mov r0, #5
    mov r1, #4
    subs r0, r1
    rsb r0, r0 ,r1

    swi 0x123456

.data

