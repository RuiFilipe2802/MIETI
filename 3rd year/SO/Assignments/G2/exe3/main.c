#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

#define NFILHOS 10

int main(){

  int i=0;
  int status;

  for(i=0;i!=NFILHOS;i++){

    pid_t pid;
    pid_t pidl;

    pid = fork();

    switch(pid){

      case -1:   //ERRO
        perror("fork");
        return 1;

      case 0:  //FILHO
        printf("filho i = %d, pid = %d, ppid = %d\n", i+1, getpid(), getppid());
        _exit(i+1);

      default:  //PAI
        printf("Criado pid: %d\n", pid);
        pidl = wait(&status);
        printf("Morreu pid: %d\n", pidl);
        if(WIFEXITED(status)){
          printf("Exit = %d\n", WEXITSTATUS(status));
        }else{
            printf("Não terminou com exito\n");
        }
      }
    }

  return 0;
}
