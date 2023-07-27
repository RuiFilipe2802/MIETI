#include <stdio.h>
#include <stdlib.h>

int main(){

     int i=0;
     int j=0;
     int aux=0;
     int numero[10];
     int a=0;

     printf ("Escreva um numero");
     scanf ("%d",&a);

     for(i=0; i<a; i++){
          printf("\nDigite um valor: ");
          scanf("%d",&numero[i]);
          }

          for(i=0; i<a; i++ ){
                  for(j=i+1;j<10;j++){
                       if( numero[i]>numero[j]){
                           aux=numero[i];
                           numero[i]=numero[j];
                           numero[j]=aux;
                       }
                  }
           }

        for(i=0;i<=a-1;i++){
          printf("\n%d",numero[i]);
        }
}

