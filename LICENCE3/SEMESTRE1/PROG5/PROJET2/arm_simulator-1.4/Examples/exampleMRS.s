.global main
.text

main:
    mrs  r0,cpsr
    mrs r1,spsr
	
end:
    swi 0x123456
.data
