#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MAX 50

FILE *lib;

// struct que da corpo a lista ligada
typedef struct library{
    char titulo[MAX];
    int ISBN;
    int qtd;
    struct library *prox;
}tProd;

tProd *cabeca=NULL; // cabeca da lista
tProd *now=NULL;   // posicao atual
tProd *theonebefore=NULL;   //anterior

tProd *create(char title[],int cod,int qnt){
    tProd *lista=(tProd*)malloc(sizeof(tProd));
    strcpy(lista->titulo,title);
    lista->ISBN=cod;
    lista->qtd=qnt;
    lista->prox=NULL;
    cabeca=now=lista;

return lista;
}       // cria a lista

tProd *add(char title[],int cod,int qnt){
    tProd *lista=(tProd*)malloc(sizeof(tProd));
    strcpy(lista->titulo,title);
    lista->ISBN=cod;
    lista->qtd=qnt;
    lista->prox=NULL;
    now->prox=lista;
    now=lista;

return lista;
}   //adiciona na lista depois de a cabeca da msm estar criada

void print_list(){
    tProd *aux=cabeca;
    while(aux!=NULL){
        printf("Titulo: %s\nISBN: %d\nQuantidade: %d\n\n",aux->titulo,aux->ISBN,aux->qtd);
        aux=aux->prox;
    }
}   //imprime o conteudo da lista

void save_data(){
    lib=fopen("library.txt","w");
    tProd *data=cabeca;
    while(data!=NULL){
        fprintf(lib,"%s\n",data->titulo);
        fprintf(lib,"%d\n",data->ISBN);
        fprintf(lib,"%d\n\n",data->qtd);
        data=data->prox;
    }
    fclose(lib);
}      // guarda o conteudo da lista num ficheiro

void recover_data(){
    char title[MAX];
    int cod=0;
    int qnt=0;
    int ini=0;
    int rod=1;
    int i=1;
    lib=fopen("library.txt","r");

    while(!feof(lib)){
        fgets(title,MAX,lib);
        rod=strlen(title);
        title[rod-1]='\0';
        fscanf(lib,"%d\n",&cod);
        fscanf(lib,"%d\n\n",&qnt);
        if(ini==0){
            create(title,cod,qnt);
            ini=1;
        }else{
            add(title,cod,qnt);
        }
    }
    fclose(lib);
} //recupera o conteudo do ficheiro pra lista

tProd *search_bar(int cod){
    tProd *seak=cabeca; //variavel pra procurar o livro pretendido
    int finally=0;

    while(seak!=NULL){
        if(seak->ISBN==cod){
            finally=1;
            break;
        }else{
            theonebefore=seak;
            seak=seak->prox;
        }
    }

    if(finally==1){
        return seak;
    }else{
        return NULL;
    }

}   //procura na lista ligada o livro escolhido pelo user

tProd *order(int qnt, tProd **porder){

    tProd *check=*porder;
    if(check->qtd<qnt){
        printf("A quantidade pretendida e superior a existente.");
        return;
    }else{
        check->qtd=(check->qtd)-qnt;
    }
} // faz a requesicao desejada

tProd *change_it_all(tProd **porder){
    tProd *alter=*porder;
    int desire;
    char title[MAX];
    int cod,qnt;
    int hope=1;

    while(hope==1){
        printf("O que deseja alterar?\n  [1]TITULO\n  [2]ISBN\n  [3]QUANTIDADE\n\nOpcao: ");
        scanf("%d",&desire);

        if(desire==1){
            printf("\n\nIntroduza o novo titulo: ");
            getchar();
            gets(title);
            strcpy(alter->titulo,title);
        }
        if(desire==2){
            printf("\n\nIntroduza o novo ISBN: ");
            scanf("%d",&cod);
            alter->ISBN=cod;
        }
        if(desire==3){
            printf("\n\nIntroduza a nova quantidade: ");
            scanf("%d",&qnt);
            alter->qtd=qnt;
        }

         printf("\n\nTitulo: %s\nISBN: %d\nQuantidade: %d\n\n",alter->titulo,alter->ISBN,alter->qtd);
         printf("Deseja fazer outra alteracao?\n  [1]SIM    [2]NAO\n\nOpcao: ");
         scanf("%d",&hope);
         if(hope==1){
            system("cls");
         }else{
            return;
         }

    }

} //altera na lista o que o user desejar

tProd *delet_som(tProd **porder){
    tProd *actual=*porder;
    if (theonebefore!=NULL){
        theonebefore->prox=actual->prox;
    }
    if (actual==cabeca){
        cabeca=actual->prox;
    }
    free(actual);
    actual=NULL;
}   // apaga o conteudo que o user escolhe

int main (){

    recover_data();
    tProd *lista=cabeca;
    tProd *porder=NULL; //variave da lista como auxiliar
    int menu=0;
    int input=0;
    int cod=0;
    char title[MAX];
    int qnt=0;
    int op=0;
    int rest=0;
    int i=0;
    int restart=1;

    while(menu==0){
        printf("\n\n                MENU                \n\n");
        printf(" 1- Registar livros.\n");
        printf(" 2- Ver library.\n");
        printf(" 3- Requesitar livros.\n");
        printf(" 4- Alterar library.\n");
        printf(" 0- Fechar library.\n");


        printf("\n\n Introduza a opcao:  ");
        scanf("%d",&input);
        system("cls");

    switch(input){

    case 0:
        if(input==0){
            save_data();
            return 0;
        }

    case 1:
        if(input==1){
            op=1;
            while(op==1){
                printf("Introduza o titulo: ");
                getchar();
                gets(title);
                printf("Introduza o codigo ISBN: ");
                scanf("%d",&cod);
                printf("Introduza a quantidade: ");
                scanf("%d",&qnt);
                system("cls");
                if((lista==NULL)&&(rest==0)){
                    create(title,cod,qnt);
                    rest=1;
                }else{
                    add(title,cod,qnt);
                }
                printf("Deseja continuar a registar livros?\n [1] SIM    [2]NAO\n\nOpcao: ");
                scanf("%d",&op);
                system("cls");

            }
            save_data();
            printf("Deseja voltar ao menu principal?\n  [1]SIM     [0]NAO\n\nOpcao: ");
            scanf("%d",&op);
            if(op==1){
                system("cls");
            }else{
                return 0;
            }

        }
    case 2:
        if(input==2){
            print_list();
            printf("Deseja voltar ao menu principal?\n  [1]SIM     [0]NAO\n\nOpcao: ");
            scanf("%d",&op);
            if(op==1){
                system("cls");
            }else{
                return 0;
            }
        }

    case 3:
        if(input==3){
            print_list();
            printf("Introduza o codigo ISBN do livro que deseja requesitar e a quantidade.\nISBN: ");
            scanf("%d",&cod);
            printf("Quantidade: ");
            scanf("%d",&qnt);
            porder=search_bar(cod);
            order(qnt,&porder);
            if(porder->qtd==0){
                delet_som(&porder);
            }
            save_data();
            printf("Deseja voltar ao menu principal?\n  [1]SIM     [0]NAO\n\nOpcao: ");
            scanf("%d",&op);
            if(op==1){
                system("cls");
            }else{
                return 0;
            }
        }
    case 4:
        if(input==4){
            printf("Deseja alterar ou remover algum artigo?\n  [1]ALTERAR      [2]REMOVER\n\nOpcao: ");
            scanf("%d",&op);
            system("cls");
            print_list();
            printf("Escolha pelo codigo ISBN, qual o artigo desejado:\nISBN: ");
            scanf("%d",&cod);
            porder=search_bar(cod);
            if(porder==NULL){
                    printf("\nO CODIGO INTRODUZIDO NAO CORRESPONDE A NENHUM LIVRO PRESENTE NA BIBLIOTECA.");
                    for (i=1;i<5;i++){
                        sleep(1);
                        fflush(stdout);
                    }
                    system("cls");
                }else{
                    if(op==1){
                        system("cls");
                        change_it_all(&porder);
                        save_data();
                        printf("\n\nDeseja voltar ao menu principal?\n  [1]SIM     [0]NAO\n\nOpcao: ");
                        scanf("%d",&op);
                        if(op==1){
                            system("cls");
                        }else{
                            return 0;
                        }
                    }
                    if(op==2){
                        system("cls");
                        delet_som(&porder);
                        save_data();
                        printf("\n\nDeseja voltar ao menu principal?\n  [1]SIM     [0]NAO\n\nOpcao: ");
                        scanf("%d",&op);
                        if(op==1){
                            system("cls");
                        }else{
                            return 0;
                        }
                    }
                }
        }

    }
    }

}
