#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {

  int i;
  pid_t pid;
  for(i = 0; i!= 10 && (pid=fork()) == 0; i++){
    printf("%d, %d\n", getpid(), getppid());
  }
  wait(NULL);
  return 0;
}
