#include <REG51F380.H>

extrn code (displaynum, displaylet, rd_btn1,rd_btn2, init_btns, operando1, operando2, operacao, delay500ms)
	
	; R3 = op1
	; R5 = operacao
	; R6 = op2
	; R7 = Resultado
	
	mov PCA0MD, #0
	mov XBR1, #40h
	
	nextstate data 30h
	ready 	  equ 0h
	select 	  equ 1h
	op2		  equ 2h
	process   equ 3h
		
	lcall init_btns
	mov nextstate, #ready
	mov r2, #0
		
	Main:
		mov a, nextstate					
		mov dptr, #jump_states
		rl a
		jmp @a+dptr
	
	Jump_States:
		ajmp Sready
		ajmp Sselect
		ajmp Sop2
		ajmp Sprocess
		
	Sready:
		mov p2, #0F9H
		lcall delay500ms
		lcall Operando1
		mov nextstate, #select
		jmp main
		
	Sselect:
		mov p2, #088h
		lcall delay500ms
		lcall delay500ms
		lcall Operacao
		mov nextstate, #op2	
		jmp main
		
	Sop2:
		mov p2, #0A4H
		lcall delay500ms
		lcall delay500ms
		lcall operando2
		mov nextstate, #process
		jmp main
	
	Sprocess:
		mov a,r5
		mov dptr, #jump_staes
		rl a
		jmp @a+dptr
	
	Jump_Staes:
		ajmp andd
		ajmp orr
		ajmp nott
		ajmp xott
		ajmp ad
		ajmp su
		ajmp rol
		ajmp ror
		ajmp main
		
	andd:
		mov a,r3
		anl	a,r6
		mov r7,a
		mov p4,a
		lcall displaynum
		lcall delay500ms
		lcall delay500ms
		mov nextstate, #ready
		jmp main

	orr: 
		mov a,r3
		orl	a,r6
		mov r7,a
		mov p4,a
		lcall displaynum
		lcall delay500ms
		lcall delay500ms
		mov nextstate, #ready
		jmp main
	
	nott:	
		mov a,r3
		CPL a
		mov r7,a
		mov p4,a
		lcall displaynum
		lcall delay500ms
		lcall delay500ms
		mov nextstate, #ready
		jmp main
	
	xott:
		mov a,r3
		XRL a,r6
		mov r7,a
		mov p4,a
		lcall displaynum
		lcall delay500ms
		lcall delay500ms
		mov nextstate, #ready
		jmp main
	
	ad:
		mov a,r3
		add a,r6
		mov r7, a
		lcall displaynum
		lcall delay500ms
		lcall delay500ms
		mov nextstate, #ready
		jmp main	

	su:
		mov a,r3
		subb a,r6
		inc a
		mov r7, a
		lcall displaynum
		lcall delay500ms
		lcall delay500ms
		mov nextstate, #ready
		jmp main

	rol:
		mov a, r3
		RL 	a
		mov r7,a
		mov p4,a
		lcall displaynum
		lcall delay500ms
		lcall delay500ms
		mov nextstate, #ready
		jmp main

	ror:
		mov a, r3
		rr 	a
		mov R7,a
		mov p4,a
		lcall displaynum
		lcall delay500ms
		lcall delay500ms
		mov nextstate, #ready
		jmp main

end
	
	
	