#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const *argv[]) {

  int fildes[2];
  pid_t pid;
  char* linha1 = "primeira linha de um pipe\n";
  char linha2[1024];

  if(pipe(fildes) == -1){
    perror("pipe");
    return 1;
  }

  pid = fork();

  switch(pid){
    case -1:
      perror("fork");
      return 1;
    case 0:
      close(fildes[1]);
      int n = read(fildes[0], linha2, sizeof(linha2));
      write(1, linha2, n);
      close(fildes[0]);
      _exit(1);
    default:
      close(fildes[0]);
      sleep(5);
      write(fildes[1], linha1, strlen(linha1));
      close(fildes[1]);
      wait(NULL);
  }

  return 0;
}
