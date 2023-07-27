#include <stdio.h>
#include <stdlib.h>

int main(){

    int n=0;
    int i=0;
    int vetor[i];

    printf("Digite o valor de n: ");
    scanf("%d", &n);

    for (i=0; i<n; i++)
      scanf("%d", &vetor[i]);

      for(i=0; i<n; i++)
        scanf("%d", &vetor[i]);
        if(vetor[i] %2==0){
      vetor[i]=2*vetor[i]+1;
        }else{
        vetor[i]=2*vetor[i]+2;
        }
}
