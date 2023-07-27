#include <stdio.h>
#include <stdlib.h>

int simetrico(int a){
 return (0-a);
 }


int main(){
  int a=0;
  printf ("esolha um numero",&a);
  scanf ("\n%d",&a);
  printf ("%d",simetrico(a));


}


