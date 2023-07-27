#include <stdio.h>
#include <stdlib.h>

int resultado (int x){
        if (x<0){
        return -1;
        }else{
        return 1;
        }
}
int main(){
    int x=0;


    printf ("escolha um numero");
    scanf  ("\n%d",&x);
    printf ("%d",resultado(x));



}
