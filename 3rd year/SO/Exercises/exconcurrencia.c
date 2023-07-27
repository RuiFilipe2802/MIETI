#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#define NFILHOS 10

int main(int argc, char const *argv[]) {

  int i=0;
  int status;
  pid_t pid;
  for(i=0;i!=NFILHOS;i++){
    pid = fork();
    switch(pid){
      case -1:  //ERRO
        perror("fork");
        return 1;
      case 0:   //FILHO
        printf("id: %d,filho: %d, pai: %d\n", i+1,getpid(), getppid());
        _exit(i+1);
      default:  //PAI
        printf("filho criado: %d\n", pid);
    }
  }

  for(i=0;i!=NFILHOS;i++){
        pid = wait(&status);
        if(WIFEXITED(status)){
          printf("exit: %d\n", WEXITSTATUS(status));
        }
  }

  return 0;
}
