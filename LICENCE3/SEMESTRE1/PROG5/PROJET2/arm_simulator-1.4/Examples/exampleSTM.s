.global main
.text

main:
    mov r0, #150
    mov r1, #0x25
    mov r2, #0x35
    mov r3, #0x45
    mov r4, #0x55   
    mov r5, #0x65
    mov r6, #0x75   
    stm r0!,{r1-r4,r5,r6} 
    mov r0, #150     
    mov r1, #0
    mov r2, #0
    mov r3, #0
    mov r4, #0  
    mov r5, #0
    mov r6, #0 
    ldm r0,{r1-r4,r5,r6} 
       
    swi 0x123456

    
 
