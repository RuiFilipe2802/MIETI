#include <stdio.h>
#include <stdlib.h>

int character(char a,int b){
    while(a<b){
        printf("%c\n",a);
        a=a+1;
    }
}

int main(){
    int n=0;
    char a=0;
    char b=0;
    printf("escreva um caracter:");
    scanf("\n%c",&a);
    printf("escreva um numero:");
    scanf("\n%d",&n);
    b=a+n;
    printf("%c",character(a,b));



}
