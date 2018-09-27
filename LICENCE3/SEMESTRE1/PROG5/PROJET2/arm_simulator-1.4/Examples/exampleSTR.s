.global main
.text

main:
    mov r0, #37
    mov r1, #4080
    str r1, [r0]
    ldrb r2, [r0]

    
    mov r1, #4294967295
    strb r1, [r0]
    ldr r2, [r0]
   
    mov r1, #10
    mov r5, #0x25	
    str r1, [r5]
    ldr r6, =donnee
    swi 0x123456

.data
donnee:
    .word 0x25
    
    


