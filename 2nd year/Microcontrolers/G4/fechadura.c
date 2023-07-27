#include <REG51F380.H>
#include <stdio.h>
#include "init.h"
#define hw_flag P0^0
sbit pisca = P2^7;
sbit scroll = P0^6;
sbit enter = P0^7;
sbit saida = P0^1;

int	bt1_press = 0;
int	bt1_aux = 0;
int bt2_press = 0;
int	bt2_aux = 0;
int o_press = 0;
int c_press = 0;
char o;
int i = 0;
int j = 0;
char display[10]={0x0C0, 0x0F9, 0x0A4, 0x0B0, 0x099, 0x092, 0x082, 0x0F8, 0x080, 0x090};
int codUti[4];
int codPre[] = {16,0,10,2};
int codNor[] = {16,0,10,2}; 
int estado = 0;
int errado = 0;
int tempo = 10;
code char estado1[] = "\n\rFechado\r\n";
code char estado2[] = "\n\rAberto\r\n";
code char estado3[] = "\n\rCodigo errado\r\n";
code char estado4[] = "\n\rUltrapassou 5 tentativas\r\n";



void botaoScroll(){ // Rotina para verificar se o botao Scroll foi premido
		while(1){	
					if( scroll == 1){
						if(bt1_aux == 1){
							break;
						}
						bt1_aux = 1;
					}else{
								if(bt1_aux == 0){
									break;
								}
								bt1_press = 1;
								bt1_aux = 0;
					}
		}
}


void botaoEnter(){ // Rotina para verificar se o botao Enter foi premido
		while(1){	
					if( enter == 1){
						if(bt2_aux == 1){
							break;
						}
						bt2_aux = 1;
					}else{
								if(bt2_aux == 0){
									break;
								}
								bt2_press = 1;
								bt2_aux = 0;
					}
		}
}

void lerTecl(){
		o	= _getkey();
			if( o == 'o'){
				o_press = 1;		
			}else if( o == 'c'){
				c_press = 1;
			}
			
}

void DigCodigo(){ // Rotina para digitar o codigo 
	  i = 0;
		j = 0;
		P2 = display[i];
		while(1){						
				  botaoScroll();
					botaoEnter();
					lerTecl();
					if(bt1_press == 1){
						i++;
						if(i == 10){
								i = 0;
						}
						bt1_press = 0;
						P2 = display[i];
					}
					if(bt2_press == 1){
						codUti[j] = 2*i;
						i = 0;
						j++;
						bt2_press = 0;
						P2 = display[i];
					if(j == 4){	
						break;
					}
				}

	}
}

void timer_delay(){	

	TH0 = 0x0b1;		
	TL0 = 0x0e0;		
	TR0 = 1;		
	while(TF0 == 0);	
	TR0 = 0;		
	TF0 = 0;		
}

void main(){
	Init_Device();
	printf(estado1);
	if(hw_flag == 1){ // reset para o codigo original caso hw_flag == 1
	codNor[0] = codPre[0];
	codNor[1] = codPre[1];
	codNor[2] = codPre[2];
	codNor[3] = codPre[3];
	}
	while(1){	
	switch(estado){
		case 0:
			P2=0x0C7;
			TMOD = 0x01;		
			for(j=0; j<7;j++){
					pisca = ~pisca;	
					for(i=0; i<0x32; i++){	
					timer_delay();	
							}
			}
			estado = 1;
		break;
		case 1:
				DigCodigo();
				lerTecl();
				if( o_press == 1){
					estado = 2;
					o_press = 0;
					break;
				}
				if( c_press == 1){
					estado = 3;
					c_press = 0;
					break;
				}
				if(codUti[0] == codNor[0] && codUti[1] == codNor[1] && // Verifica o codigo e compara com o codigo guardado
				   codUti[2] == codNor[2] && codUti[3] == codNor[3]){
					P2 = 0x0A3;
					estado = 2;	
				}else{
						estado = 3;
				}
				break;
		 
		case 2:
				while(1){
							printf(estado2);
							botaoEnter();
							botaoScroll();
						if( bt2_press == 1){ // Mantem o mesmo codigo
							estado = 0;
							codNor[0] = codUti[0];
							codNor[1] = codUti[1];
							codNor[2] = codUti[2];
							codNor[3] = codUti[3];
							bt2_press = 0;
							estado = 0;
								break;
						}else if(bt1_press == 1){ // guarda o novo codigo
										bt1_press = 0;
										DigCodigo();
										codNor[0] = codUti[0];
										codNor[1] = codUti[1];
										codNor[2] = codUti[2];
										codNor[3] = codUti[3];
										estado = 0;
							break;
							}	
			}
			break;
			
		case 3:					// Apresenta F no display e avanca para o estado 4 caso errado == 5
				printf(estado3);
				P2=0x08E;
				errado++;
				tempo = tempo+10;
				TMOD = 0x01;		
				for(j=0; j<tempo; j++){
					pisca = ~pisca;	
					for(i=0; i<0x32; i++){	
					timer_delay();	
							}
				}
			estado = 0;
						if(errado==5){
							estado = 4;
							tempo = 10;
						}
			break;
			
		case 4:				// Apresenta b no display ate que hw_flag == 1
				printf(estado4);
				P2=0x083; 
				while(hw_flag == 0){					
					saida = 1;
				}
			errado = 0;
			saida = 0;
			estado = 2;
			tempo = 10;
				break;
		}
	}
}