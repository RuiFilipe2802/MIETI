#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MAX 50
#define SMAX 100

FILE *lr;       //declaracao do file

typedef struct {
    char ap_nome[MAX];
    char ap_paragens[MAX];

}ComboiosAp; // declara tipo e paragens dos comboios
ComboiosAp AP[SMAX];

char aps_nome[][MAX]={"AP_102","AP_204","AP_320","AP_450"};

typedef struct {
    char ic_nome[MAX];
    char ic_paragens[MAX];
}ComboioIC;
ComboioIC IC[SMAX];

char ics_nome[][MAX]={"IC_089","IC_160","IC_203","IC_344"};
char paragens[][MAX]={"Braga","Famalicao","Porto","Aveiro","Coimbra","Entroncamento","Santarem","Lisboa"};

void def_variaveis(){
    int i;

    for(i=0;i<4;i++){
        strcpy(AP[i].ap_nome,aps_nome[i]);
        strcpy(IC[i].ic_nome,ics_nome[i]);
    }
    for(i=0;i<8;i++){
        strcpy(AP[i].ap_paragens,paragens[i]);
        strcpy(IC[i].ic_paragens,paragens[i]);
    }
} // funcao que atribui os nomes e paragens para as funcoes

int tp(int *times){

    int hours, minutes;
	time_t now;
    	time(&now);

	struct tm *local = localtime(&now);

	hours = local->tm_hour;
	minutes = local->tm_min;

    *times=(hours%100)*100+(minutes%100);


return *times;
} // verifica a hora do sistema

void first_case(int AP_H[8][4], int IC_H[8][4], int *hora, int *minutos, int *partida, int *destino,int *aux_AP,int *aux_IC,int *tipe){

        int i;
        int tempo_M=(*hora)*60+(*minutos);
        int horas, min;
        int reg_AP=0;
        int reg_IC=0;

            for(i=0;i<4;i++){
                if(AP_H[*partida][i]>=tempo_M){
                    *aux_AP=i;
                    break;
                }
                if((i==3)&&(AP_H[*partida][i]<tempo_M)&&(tempo_M<=1439)){
                    tempo_M=0;
                    i=0;
                    reg_AP=1;
                    *aux_AP=0;
                    break;
                }

            }
            tempo_M=(*hora)*60+(*minutos);
            for(i=0;i<4;i++){
                if(IC_H[*partida][i]>=tempo_M){
                    *aux_IC=i;
                    break;
                }
                if((i==3)&&(IC_H[*partida][i]<tempo_M)&&(tempo_M<=1439)){
                    tempo_M=0;
                    i=0;
                    reg_IC=1;
                    *aux_IC=0;
                    break;
                }
            }


        if((reg_AP!=1)&&(reg_IC!=1)||(reg_AP==1)&&(reg_IC==1)){
            if((AP_H[*partida][*aux_AP])<(IC_H[*partida][*aux_IC])){
                horas=AP_H[*partida][*aux_AP]/60;
                min=AP_H[*partida][*aux_AP]%60;
                printf("\n\nO comboio mais proximo sera o %s.\n",AP[*aux_AP].ap_nome);
                printf("Partida de %s as %02d:%02dh.\n",AP[*partida].ap_paragens,horas,min);
                horas=AP_H[*destino][*aux_AP]/60;
                min=AP_H[*destino][*aux_AP]%60;
                printf("Chegada a %s as %02d:%02dh.\n\n",AP[*destino].ap_paragens,horas,min);
                *tipe=0;
            }
            if((AP_H[*partida][*aux_AP])>(IC_H[*partida][*aux_IC])){
                horas=IC_H[*partida][*aux_IC]/60;
                min=IC_H[*partida][*aux_IC]%60;
                printf("\n\nO comboio mais proximo sera o %s.\n",IC[*aux_IC].ic_nome);
                printf("Partida de %s as %02d:%02dh.\n",IC[*partida].ic_paragens,horas,min);
                horas=IC_H[*destino][*aux_IC]/60;
                min=IC_H[*destino][*aux_IC]%60;
                printf("Chegada a %s as %02d:%02dh.\n\n",IC[*destino].ic_paragens,horas,min);
                *tipe=1;
            }
        }

        if((reg_AP==1)&&(reg_IC!=1)){
            horas=IC_H[*partida][*aux_IC]/60;
            min=IC_H[*partida][*aux_IC]%60;
            printf("\n\nO comboio mais proximo sera o %s.\n",IC[*aux_IC].ic_nome);
            printf("Partida de %s as %02d:%02dh.\n",IC[*partida].ic_paragens,horas,min);
            horas=IC_H[*destino][*aux_IC]/60;
            min=IC_H[*destino][*aux_IC]%60;
            printf("Chegada a %s as %02d:%02dh.\n\n",IC[*destino].ic_paragens,horas,min);
            *tipe=1;
        }
        if((reg_AP!=1)&&(reg_IC==1)){
            horas=AP_H[*partida][*aux_AP]/60;
            min=AP_H[*partida][*aux_AP]%60;
            printf("\n\nO comboio mais proximo sera o %s.\n",AP[*aux_AP].ap_nome);
            printf("Partida de %s as %02d:%02dh.\n",AP[*partida].ap_paragens,horas,min);
            horas=AP_H[*destino][*aux_AP]/60;
            min=AP_H[*destino][*aux_AP]%60;
            printf("Chegada a %s as %02d:%02dh.\n\n",AP[*destino].ap_paragens,horas,min);
            *tipe=0;
        }

}

void expanded_case(int AP_H[6][4], int IC_H[8][4], int *hora, int *minutos, int *partida, int *destino,int *aux_AP, int *aux_IC,int *tipe){
        int i;
        int horas,min;

        for(i=*partida;i<=*destino;i++){

            if(*tipe==0){
                horas=AP_H[i][*aux_AP]/60;
                min=AP_H[i][*aux_AP]%60;
                printf("%02d:%02dh ->  %s\n",horas,min,AP[i].ap_paragens);
            }
            if(*tipe==1){
                horas=IC_H[i][*aux_IC]/60;
                min=IC_H[i][*aux_IC]%60;
                printf("%02d:%02dh ->  %s\n",horas,min,IC[i].ic_paragens);
            }
        }


}

void second_case(int AP_H[8][4],int IC_H[8][4],int *hora, int *minutos){

        int i,j;
        int choice;

        printf("\n\n Qual a linha de comboios quer apresentar?\n[0] AP - alpha   \n[1] IC - intercidade   \n[2] Ambos\n\nOpcao: ");
        scanf("%d",&choice);
        for(i=0;i<5;i++){
            if((choice<0)||(choice>2)){
                printf("\nOpcao invalida. Introduza novamente.\nOpcao: ");
                scanf("%d",&choice);
            }else{
                break;
            }
        }
        if(choice==0){
            printf("\n\n  ");
            for(i=0;i<4;i++){
                printf("%s   ",AP[i].ap_nome);
            }
            for(i=0;i<8;i++){
                printf("\n   ");
                for(j=0;j<4;j++){
                    *hora=AP_H[i][j]/60;
                    *minutos=AP_H[i][j]%60;
                    printf("%02d:%02d    ",*hora,*minutos);
                }
                printf("%s\n",AP[i].ap_paragens);
            }
            printf("\n\n");

        }
        if(choice==1){
            printf("\n\n  ");
            for(i=0;i<4;i++){
                printf("%s   ",IC[i].ic_nome);
            }
            for(i=0;i<8;i++){
                printf("\n   ");
                for(j=0;j<4;j++){
                    *hora=IC_H[i][j]/60;
                    *minutos=IC_H[i][j]%60;
                    printf("%02d:%02d    ",*hora,*minutos);
                }
                printf("%s\n",IC[i].ic_paragens);
            }
            printf("\n\n");

        }
        if(choice==2){
            printf("\n\n  ");
            for(i=0;i<4;i++){
                printf("%s   ",AP[i].ap_nome);
            }
            for(i=0;i<6;i++){
                printf("\n   ");
                for(j=0;j<4;j++){
                    *hora=AP_H[i][j]/60;
                    *minutos=AP_H[i][j]%60;
                    printf("%02d:%02d    ",*hora,*minutos);
                }
                printf("%s\n",AP[i].ap_paragens);
            }
            printf("\n\n");

            for(i=0;i<4;i++){
                printf("  %s ",IC[i].ic_nome);
            }
            for(i=0;i<8;i++){
                printf("\n   ");
                for(j=0;j<4;j++){
                    *hora=IC_H[i][j]/60;
                    *minutos=IC_H[i][j]%60;
                    printf("%02d:%02d    ",*hora,*minutos);
                }
                printf("%s\n",IC[i].ic_paragens);
            }
            printf("\n\n");

        }

}

int main(){

    int i=0;
    int times;
    int AP_H[8][4]={{241,601,841,1081},                    //{0401,1001,1401,1801}
                    {257,617,857,1097},                    //{0417,1017,1417,1817}
                    {280,640,880,1120},                    //{0440,1040,1440,1840}
                    {315,675,915,1155},                    //{0515,1115,1515,1915}
                    {344,704,944,1184},                     //{0544,1144,1544,1944}
                    {348,708,948,1188},                     //{0548,1148,1548,1948}
                    {367,727,967,1207},                     //{0607,1207,1607,2007}
                    {438,805,1042,1282}};                   //{0718,1325,1722,2122}


    int IC_H[8][4]={{485,785,965,1205},                     //{0805,1305,1605,2005}
                    {501,801,981,1221},                     //{0821,1321,1621,2021}
                    {525,825,1005,1245},                    //{0845,1345,1645,2045}
                    {565,865,1045,1285},                    //{0925,1425,1725,2125}
                    {597,897,1077,1317},                    //{0957,1457,1757,2157}
                    {661,961,1141,1381},                    //{1101,1601,1901,2301}
                    {680,980,1160,1400},                    //{1120,1620,1920,2320}
                    {708,1012,1197,1425}};                  //{1148,1652,1457,2345}

    def_variaveis();
    tp(&times);

    int input;
    int partida;
    int destino;
    int escolha;
    int hora,minutos,tempo;
    int aux_AP,aux_IC;
    int ret_menu;
    int menu=0;
    int choice;
    int tipe;

    while(menu==0){

    printf("*********************************\n");
    printf("------- Choose an option: -------\n");
    printf("                                 \n");
    printf(" [1]  ->  Partida e Destino      \n");
    printf(" [2]  ->  Horarios               \n");
    printf(" [3]  ->  Comboio mais rapido    \n");
    printf(" [4]  ->  Ultima pesquisa        \n");
    printf(" [0]  ->  Terminar o programa    \n");
    printf("\n*********************************\n");

    printf("\nOpcao: ");
    scanf("%d", &input);

    if(input==0){
        return 0;
    }

    switch(input){

    case 1:
        if(input==1){
            system ("cls");
            printf("Partida:               \n");
            for(i=0;i<7;i++){
                printf("[%d] -> %s\n",i,IC[i].ic_paragens);
            }
            printf("\nOpcao: ");
            scanf("%d",&partida);
            for(i=0;i<5;i++){
                if((partida<0)||(partida>6)){
                    printf("Opcao invalida. Escolha novamente.\n\nOpcao: ");
                    scanf("%d",&partida);
                }else{
                    break;
                }
            }

            printf("\nDestino: \n");
            for(i=partida+1;i<8;i++){
                printf("[%d] -> %s\n",i,IC[i].ic_paragens);
            }
            printf("\nOpcao: ");
            scanf("%d",&destino);
            for(i=0;i<5;i++){
                if((destino<partida+1)||(destino>7)){
                    printf("Opcao invalida. Escolha novamente.\n\nOpcao: ");
                    scanf("%d",&destino);
                }else{
                    break;
                }
            }

            printf("\nIntroduza:\n1-Usar hora atual\n2-Introduzir hora manualmente\n\nOpcao: ");
            scanf("%d",&escolha);

            for(i=0;i<5;i++){
                if((escolha<1)||(escolha>2)){
                    printf("Opcao invalida. Escolha novamente.\n");
                    scanf("%d",&escolha);
                }else{
                    break;
                }
            }

            if(escolha==1){
                hora=times/100;
                minutos=times%100;
            }
            if(escolha==2){
                for(i=0;i<10;i++){
                    printf("Horas: ");
                    scanf("%d",&hora);
                    if((hora>=0)&&(hora<24)){
                        break;
                    }else{
                        printf("Hora introduzida inválida. Introduza novamente.\n");
                    }
                }
                for(i=0;i<10;i++){
                    printf("Minutos: ");
                    scanf("%d",&minutos);
                    if((minutos>=0)&&(minutos<60)){
                        break;
                    }else{
                        printf("Hora introduzida invalida. Introduza novamente.\n");
                    }
                }

            }

            lr=fopen("research.bin","wb");

            if(lr==NULL){
                printf("Error Opening file.");
                exit(1);
            }
            fwrite(&partida,sizeof(1),1,lr);
            fwrite(&destino,sizeof(1),1,lr);
            fwrite(&hora,sizeof(1),1,lr);
            fwrite(&minutos,sizeof(1),1,lr);

            fclose(lr);

            system("cls");

            printf("\n\n\n\n\n\n                        LOADING ");
            for (i=1;i<4;i++){
                sleep(1);
                printf(".");
                fflush(stdout);
            }
            system("cls");

            printf("\nPARTIDA: %s.   DESTINO: %s.    HORAS: %02d:%02d.\n",IC[partida].ic_paragens,IC[destino].ic_paragens,hora,minutos);

            first_case(AP_H,IC_H,&hora,&minutos,&partida,&destino,&aux_AP,&aux_IC,&tipe);

            printf("Deseja apresentar a extencao do horario?\n[0]Nao    [1]Sim\n\nOpcao: ");
            scanf("%d",&choice);

            if(choice==1){
                system("cls");
                printf("\nPARTIDA: %s.   DESTINO: %s.    HORAS: %02d:%02d.\n",IC[partida].ic_paragens,IC[destino].ic_paragens,hora,minutos);

                first_case(AP_H,IC_H,&hora,&minutos,&partida,&destino,&aux_AP,&aux_IC,&tipe);
                expanded_case(AP_H,IC_H,&hora,&minutos,&partida,&destino,&aux_AP,&aux_IC,&tipe);
            }


            printf("\nDESEJA RETORNAR AO MENU ANTERIOR?\n[0]NAO   [1]SIM\n\nOpcao: ");
            scanf("%d",&ret_menu);

            if(ret_menu==1){
                system("cls");
            }else{
                return;
            }

        break;
        }

    case 2:
        if(input==2){
            system("cls");
            printf("\n\n\n\n\n\n                        LOADING ");
            for (i=1;i<4;i++){
                sleep(1);
                printf(".");
                fflush(stdout);
            }
            system("cls");
            second_case(AP_H,IC_H,&hora,&minutos);

            printf("DESEJA RETORNAR AO MENU ANTERIOR?\n[0]NAO   [1]SIM\n\nOpcao: ");
            scanf("%d",&ret_menu);


            if(ret_menu==1){
                system("cls");
            }else{
                return;
            }

        break;
        }

    case 3:
        if(input==3){
            system("cls");
            printf("\n\n\n\n\n\n                        LOADING ");
            for (i=1;i<4;i++){
                sleep(1);
                printf(".");
                fflush(stdout);
            }
            system("cls");

            partida=0;
            destino=7;
            hora=times/100;
            minutos=times%100;

            printf("\nPARTIDA: %s.   DESTINO: %s.    HORAS: %02d:%02d.\n",IC[partida].ic_paragens,IC[destino].ic_paragens,hora,minutos);

            first_case(AP_H,IC_H,&hora,&minutos,&partida,&destino,&aux_AP,&aux_IC,&tipe);

            printf("Deseja apresentar a extencao do horario?\n[0]Nao    [1]Sim\n\nOpcao: ");
            scanf("%d",&choice);

            if(choice==1){
                system("cls");
                printf("\nPARTIDA: %s.   DESTINO: %s.    HORAS: %02d:%02d.\n",IC[partida].ic_paragens,IC[destino].ic_paragens,hora,minutos);

                first_case(AP_H,IC_H,&hora,&minutos,&partida,&destino,&aux_AP,&aux_IC,&tipe);
                expanded_case(AP_H,IC_H,&hora,&minutos,&partida,&destino,&aux_AP,&aux_IC,&tipe);
            }

            printf("\nDESEJA RETORNAR AO MENU ANTERIOR?\n[0]NAO   [1]SIM\n\nOpcao: ");
            scanf("%d",&ret_menu);

            if(ret_menu==1){
                system("cls");
            }else{
                return;
            }

        break;
        }

    case 4:
        if(input==4){
            system("cls");
            printf("\n\n\n\n\n\n                        LOADING ");
            for (i=1;i<4;i++){
                sleep(1);
                printf(".");
                fflush(stdout);
            }
            system("cls");

            lr=fopen("research.bin","rb");

            if(lr==NULL){
                printf("Error Opening file.");
                exit(1);
            }
            fread(&partida,sizeof(1),1,lr);
            fread(&destino,sizeof(1),1,lr);
            fread(&hora,sizeof(1),1,lr);
            fread(&minutos,sizeof(1),1,lr);

            printf("ULTIMA PESQUISA:");
            printf("   PARTIDA: %s.   DESTINO: %s.    HORAS: %02d:%02d.\n",IC[partida].ic_paragens,IC[destino].ic_paragens,hora,minutos);

            first_case(AP_H,IC_H,&hora,&minutos,&partida,&destino,&aux_AP,&aux_IC,&tipe);

            printf("Deseja apresentar a extencao do horario?\n[0]Nao    [1]Sim\n\nOpcao: ");
            scanf("%d",&choice);

            if(choice==1){
                system("cls");
                printf("ULTIMA PESQUISA:");
                printf("   PARTIDA: %s.   DESTINO: %s.    HORAS: %02d:%02d.\n",IC[partida].ic_paragens,IC[destino].ic_paragens,hora,minutos);

                first_case(AP_H,IC_H,&hora,&minutos,&partida,&destino,&aux_AP,&aux_IC,&tipe);
                expanded_case(AP_H,IC_H,&hora,&minutos,&partida,&destino,&aux_AP,&aux_IC,&tipe);
            }
            fclose(lr);

            printf("\nDESEJA RETORNAR AO MENU ANTERIOR?\n[0]NAO   [1]SIM\n\nOpcao: ");
            scanf("%d",&ret_menu);

            if(ret_menu==1){
                system("cls");
            }else{
                return;
            }

        break;
        }
    default:
        printf("\nOpcao invalida. Introduza novamente.");
        for (i=1;i<2;i++){
                sleep(1);
                fflush(stdout);
        }
        system("cls");
    }
    }

    return;
}
