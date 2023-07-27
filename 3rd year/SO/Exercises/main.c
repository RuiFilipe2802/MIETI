#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
  int fd[2];
  if(pipe(fd)==-1){
    perror("Erro na criação do pipe");
    return 1;
  }
  switch(fork()){
    case-1:
      perror("Erro no fork");
      return 1;
    case 0:
      close(fd[1]);
      char msg[10];
      int i = read(fd[0], msg, sizeof(msg));
      write(1, msg, i);
      close(fd[0]);
      _exit(0);
    default:
      close(fd[0]);
      char *msg1 = "MENSAGEM";
      write(fd[1], msg1, strlen(msg1));
      close(fd[1]);
      wait(NULL);
  }

  return 0;
}
