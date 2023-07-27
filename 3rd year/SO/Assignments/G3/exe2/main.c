#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const *argv[]) {

  char* args[] = {"ls", "-l", NULL};

  if(fork() == 0){
    execvp("ls", args);
    perror("ls");
    printf("%d\n", getpid());
  }

  wait(NULL);
  return 1;
}
