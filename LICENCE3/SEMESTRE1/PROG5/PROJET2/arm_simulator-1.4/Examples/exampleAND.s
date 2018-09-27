.global main
.text

flags: 
    mov r0, #11
    mov r1, #7
    orr r0, r1 	
	bl end
main:
    mov r0, #7
    mov r1, #9
	and r0, r1
	
	mov r0, #5
    mov r1, #10
    eor r0, r1
    
 	mov r0, #5
    mov r1, #10
    subs r0, r1 
    
    mov r0, #5
    mov r1, #5
    cmp r0, r1 
    beq flags  
	
end:
    swi 0x123456
.data
