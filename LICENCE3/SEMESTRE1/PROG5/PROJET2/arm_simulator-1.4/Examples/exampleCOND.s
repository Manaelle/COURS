.global main
.text

ge: 
    bl end

main:
    add r0, #70
    adds r1, #64
	cmp r0, r1
	bgt ge
	
end:
    swi 0x123456
.data
