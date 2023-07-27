#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const *argv[]) {

  char *args[100];
  char linha[100];
  printf("\nmeu prompt $ ");
  fgets(linha, sizeof(linha), stdin);
  execvp("ls", linha);
}
