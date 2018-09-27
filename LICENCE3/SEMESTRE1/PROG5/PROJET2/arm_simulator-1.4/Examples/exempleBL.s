.global main
.text
main:
    bl loop
    bne main

loop:
    bl main
    bne loop
end:
    swi 0x123456
.data


.global main
.text
decr:
    subs r0, r0, #1
    mov pc, lr

main:
    mov r0, #5
    bl loop
loop:
    bl decr
    bne loop
end:
    swi 0x123456
.data

.global main
.text
main:
	bl main
    bl loop


loop:
    bl main

end:
    swi 0x123456
.data

