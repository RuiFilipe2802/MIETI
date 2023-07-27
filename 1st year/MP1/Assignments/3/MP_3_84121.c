#include  <stdio.h>
#include  <stdlib.h>

int main () {

int numero=0;
int a=0;
int b=0;
int c=0;

 printf ("Insira um número:\n");
 scanf ("%d", &numero);

 for (a=1; a<=numero; a++){
  for (b=numero; b>=a; b--)
	printf (" ");
  for (c=1; c<=2*a-1; c++)
	printf ("A");
    printf ("\n");
 }
}


