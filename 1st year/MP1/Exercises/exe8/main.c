#include <stdio.h>
#include <stdlib.h>

int main() {

  float salario=0;
  float prestacao=0;
  float valor=0;

  scanf ("%f",&salario);
  scanf ("%f",&prestacao);
  valor=0.2*salario;
  if(prestacao>valor){
        printf ("emprestimo nao pode ser concedido");
   }else{
        printf ("emprestimo pode ser concedido");
  }
}
