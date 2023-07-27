#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LINHAS 4
#define COLUNAS 10000
#define CHAVE 7

int matriz[LINHAS][COLUNAS];

int main(int argc, char const *argv[]) {

  int i; int status; pid_t pid; matriz[3][1025] = CHAVE; matriz[0][125] = CHAVE;

  for(i=0;i != LINHAS;i++){
    pid=fork();
    if(pid == 0){
        int j=0;
        for(j=0;j<COLUNAS;j++){
            if(matriz[i][j] == CHAVE){
                _exit(1);
            }else{
                _exit(0);
            }
        }
    }
  }

  for(i=0;i!=LINHAS;i++){
      pid = wait(&status);
      if(WIFEXITED(status)){
        printf("encontrou\n", WEXITSTATUS(status));
      }
  }

  return 0;
}
