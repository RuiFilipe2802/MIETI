#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const *argv[]) {

  int i;

  for(i=0;i!=5;i++){
    pid_t pid;
    pid = fork();
    switch(pid) {
      case -1 :
        perror("fork");
        return 1;
      case 0 :
        printf("filho: %d, pai: %d\n", getpid(), getppid());
        _exit(i+1);
      default:
      wait(NULL);
    }
  }

  printf("Acabou no processo pai com id :%d e pai: %d\n", getpid(), getppid());

  return 0;
}
