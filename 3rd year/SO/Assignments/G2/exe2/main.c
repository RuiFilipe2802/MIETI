#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(){

  printf("pai: pid = %d, ppid = %d\n", getpid(), getppid());
  pid_t pid = fork();
  if(pid == -1){
    perror("fork");
    return 1;
  }
  puts("ola");
  if(pid == 0){ //// FILHO
    printf("filho: pid = %d, ppid = %d\n", getpid(), getppid());
    return 1;
  }  ///// PAI
  printf("pai: pid = %d, ppid = %d, filho = %d\n", getpid(), getppid(), pid);

  //sleep(1);
  wait(NULL);
  return 0;
}
