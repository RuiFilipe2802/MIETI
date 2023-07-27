#include <stdio.h>
#include <stdlib.h>

int soma(int a,int b){
return (a+b);
}
int dobro(int a){
return (a+a);
}
int main(){
    int x=0;
    int y=0;

    printf("escolha 2 numeros");
    scanf("%d",&x);
    scanf("%d",&y);
    printf("\n%d",soma(x,y));
    printf("\n%d",dobro(x));
    printf("\n%d",dobro(y));

}


