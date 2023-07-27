#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char const *argv[]) {

  pid_t pid;
  pid = fork();
  switch(pid){
    case -1:
      perror("fork");
      return 1;
    case 0:
      execlp("ls", "ls", "-l", NULL);
      _exit(1);
    default:
    wait(NULL);
  }
  return 0;
}
