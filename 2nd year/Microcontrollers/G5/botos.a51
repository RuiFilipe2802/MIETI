#include <REG51F380.H>

		BSEG at 0h
		bt1_press: 	dbit 1
		bt1_aux: 	dbit 1
		bt2_press: 	dbit 1
		bt2_aux: 	dbit 1

public  displaynum,displaylet, rd_btn1, rd_btn2, init_btns
public  operando1, operando2,operacao, delay500ms

DEVICE SEGMENT CODE
    rseg DEVICE
		
	init_btns:
		clr		bt1_press
		clr		bt1_aux
		setb	P0.6
		clr		bt2_press
		clr		bt2_aux
		setb	P0.7
		ret

	rd_btn1:
		jb 		P0.6, bt1_one   	 ;botao P0.6 presssionado? Não, vai para bt1_one 
		jb 		bt1_aux, rd_btn1_end ;
		setb 	bt1_aux
		jmp		rd_btn1_end		
	bt1_one:
		jnb		bt1_aux, rd_btn1_end ; bt1_aux = 0 , vai para rd_btn1_end
		setb	bt1_press			; bt1_aux = 1 , bt1_press = 1 botao foi pressionado e largado
		clr		bt1_aux	
	rd_btn1_end:
		ret
	
	rd_btn2:
		jb 		P0.7, bt2_two		 ;botao P0.7 presssionado? Não, vai para bt2_two
		jb 		bt2_aux, rd_btn2_end  
		setb 	bt2_aux
		jmp		rd_btn2_end	
	bt2_two:
		jnb		bt2_aux, rd_btn2_end
		setb	bt2_press
		clr		bt2_aux	
	rd_btn2_end:
		ret

	displaynum:
		push 	acc
		mov 	dptr, #tab_displaynum
		movc 	a, @a+dptr
		mov 	P2, a
		pop 	acc
		ret	
	
	displaylet:
		push 	acc
		mov 	dptr, #tab_displaylet
		movc 	a, @a+dptr
		mov 	P2, a
		pop 	acc
		ret	

	tab_displaynum:
		DB 0C0H,0F9H,0A4H,0b0H,099H,092H,082H,0F8H,080H,090H
			
	tab_displaylet:
		DB 088h, 0c0h,0abh,089h,08ch,0c8h,0c7h,0afh
			
	Operando1:
		lcall displaynum	
		lcall rd_btn1
		lcall rd_btn2
		jb bt1_press, ince
	Operando1con:	
		jnb bt2_press, operando1
		clr bt2_press
		mov r3,a
		mov r2, #0
		mov a, #0
		ret	
	ince:
		inc r2
		mov a, r2
		cjne r2, #10, volta1
		mov r2, #0
		mov a, r2
	Volta1:
		clr bt1_press
		jmp operando1con
		
	Operacao:
		lcall displaylet
		lcall rd_btn1
		lcall rd_btn2
		jb bt1_press, inceO
	OperacaoCont:
		jnb bt2_press, operacao
		clr bt2_press
		mov r5,a 
		mov r2, #0
		mov a, #0
		ret	
	inceO:
		inc r2
		mov a, r2
		cjne r2, #8, voltaOp
		mov r2, #0
		mov a, r2
	VoltaOp:
		clr bt1_press
		jmp operacaocont
	
	Operando2:
		lcall displaynum	
		lcall rd_btn1
		lcall rd_btn2
		jb bt1_press, ince2
	Operando2con:	
		jnb bt2_press, operando2
		clr bt2_press
		mov r6,a
		mov r2, #0
		mov a, #0
		ret	
	ince2:
		inc r2
		mov a, r2
		cjne r2, #10, volta2
		mov r2, #0
		mov a, r2
	Volta2:
		clr bt1_press
		jmp operando2con
		
	delay500ms:                          
         mov R1,#0fah                
	aux1:
		mov R2,#0f9h                
        nop                                     
        nop                                     
        nop                                    
        nop                                     
		nop                                     
	aux2:
		nop                                     
        nop                                     
        nop                                     
        nop                                     
        nop                                     
        nop                                    
        djnz R2,aux2                 
        djnz R1,aux1                 
		ret 
		
end
		
		
	