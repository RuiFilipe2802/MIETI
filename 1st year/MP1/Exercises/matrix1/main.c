#include <stdio.h>
#include <stdlib.h>

int main(){
    int c=0;
    int l=0;
    int i=0;
    int j=0;

    int matriz1[i][j];
    int matriz2[i][j];
    int matrizsoma[i][j];

    printf("numero de colunas da primeira matriz");
    scanf("%d",&i);

    printf("numero de linhas da primeira matriz");
    scanf("%d",&j);

    for (i=0; i<c; i++) {
        for(j=0; j<l; j++)
            printf(" Entre com os elementos da matriz 1 [%d][%d]:",i+1,j+1);
            scanf("%d",&matriz1[i][j]);

            printf(" Entre com os elementos da matriz 2 [%d][%d]:",i+1,j+1);
            scanf("%d",&matriz2[i][j]);
    }
    matrizsoma[i][j] = matriz1[i][j] + matriz2[i][j];

    printf("\n\nA matriz 1 criada e: \n\n");
        for (i=0; i<l; i++)
{
        for(j=0; j<c;j++)
printf("%d",matriz1[i][j]);
printf("\n\n");
}

}


