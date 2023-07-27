#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {

  char* args[100], *p=NULL;
  char linha[100];
  int i=0;

  printf("meu_prompt:$ ");
  fgets(linha, sizeof(linha), stdin);
  for(p = strtok(linha, " \n"); p != NULL; p = strtok(NULL, " \n")){
    args[i] = strdup(p);
    i++;
  }
  args[i] = NULL;
  execvp(args[0], args);
  return 0;
}
