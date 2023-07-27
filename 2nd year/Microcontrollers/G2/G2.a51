#include <REG51F380.H>

	mov r0, #80h

main:
	call subst
	jmp fim
	
	subst:
		cjne r0, #30h, cont
		inc r0
		cjne r0, #128d , main
	cont:
		mov @r0, #30h
		inc r0
		cjne r0, #128 , main
		
		fim:
	end