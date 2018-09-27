.global main
.text

egal: 
    bl end

main:

    mov r0, #1
	adcs r0, r0, #4294967295
	adc r0, r1

	
end:
    swi 0x123456
.data
