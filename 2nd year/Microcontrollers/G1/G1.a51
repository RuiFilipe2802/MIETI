
#include <REG51F380.H>
BLOCK_SIZE EQU 8

	CSEG at 0h
		ljmp init


	init:	
		mov PCA0MD, #0
		mov XBR1, #40h
		ljmp main
		
		
	CSEG at 50h		
	main:
		mov 20h,#1
		mov 21h,#2
		mov 22h,#4
		mov 23h,#8
		mov 24h,#16
		mov 25h,#32
		mov 26h,#64
		mov 27h, #128
		mov r3, #BLOCK_SIZE
		mov r0, #20h
		mov r1, #30h
	loop:
		mov a, @r0
		mov @r1, a
		cpl a
		mov P2, a
		inc r0
		inc r1
		djnz r3, loop
		sjmp $
	
end