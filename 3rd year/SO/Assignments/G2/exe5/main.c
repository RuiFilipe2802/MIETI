#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

#define NFILHOS 10

int main(){

  int i;
  pid_t pid;

  for(i=0;i!=NFILHOS && (pid = fork()) == 0;i++); //NADA
    if(i!=NFILHOS)
      printf("pid == %d, filho == %d\n", getpid(), pid);
      else
      printf("pid = %d\n", getpid());
  return 0;
}
