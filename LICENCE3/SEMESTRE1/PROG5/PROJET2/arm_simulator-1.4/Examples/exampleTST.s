.global main
.text

main:
    mov r0, #15
    mvn r2, r0
    mvn r1, r2
    tst r0,r1
    mov r0, #5
    mov r1, #10
    tst r0,r1   
    
    mov r0, #10
    mov r1, #5
    teq r0,r1
    mov r0, #15
    mov r1, #15
    teq r0,r1 

    mov r0, #1
    mov r1, #4294967295
    cmn r0,r1
    mov r0, #10
    mov r1, #15
    cmn r0,r1

    mov r0, #11
    mov r1, #5
    bic r0,r1
    mov r0, #7
    mov r1, #15
    bic r0,r1

    mvn r0, #4
    mvn r1, #10
    bic r0,r1
    mvn r0, #8
    mvn r1, #0
    bic r0,r1



.data

