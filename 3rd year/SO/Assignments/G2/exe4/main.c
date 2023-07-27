#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

#define NFILHOS 10

int main(){

  int i=0;
  int status;
  pid_t pid;

  for(i=0;i!=NFILHOS;i++){

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
      }
    }
    for(i=0;i!=NFILHOS;i++){
      pid = wait(&status);
      printf("Morreu pid: %d\n", pid);
      if(WIFEXITED(status)){
        printf("Exit: %d\n", WEXITSTATUS(status));
      }else{
        printf("Filho não terminou com êxito!\n");
      }
    }
  return 0;
}
