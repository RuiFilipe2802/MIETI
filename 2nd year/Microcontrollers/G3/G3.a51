#include <REG51F380.H>
		bt1 equ P0.6
		bt2 equ P0.7
		
		CSEG at 0h
		ljmp init
		
		CSEG at 50h			
	init:	
		mov PCA0MD, #0
		mov XBR1, #40h
		setb bt1
		setb bt2

	estado1:
		mov P2, #0F9H
		jb bt1, estado1
		setb b.0
		sjmp botao1

	estado2:
		mov P2, #0A4H
		jb bt2, estado2
		setb b.1
		sjmp botao2
				
	estado3:
		mov P2, #0B0H
		jb bt2, estado3
		setb b.2
		sjmp botao3	 
		
	estado4:
		mov P2, #099H
		jb bt2, estado4
		setb b.3
		sjmp botao4
		
	botao1:
		jnb bt1, $
		clr b.0
		jmp estado2	
		
	botao2:
		jnb bt2, $
		clr b.1
		jmp estado3
	
	botao3:
		jnb bt2, $
		clr b.2
		jmp estado4	
		
	botao4:
		jnb bt2, $
		clr b.3
		jmp estado2
		
	end