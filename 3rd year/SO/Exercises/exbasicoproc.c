#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#define NFILHOS 100

int main(int argc, char const *argv[]) {

  int i=0;
  pid_t pid;
  for (i = 0; i != NFILHOS && (pid = fork()) == 0; i++) {
  }
    if(i != NFILHOS){
      printf("id: %d, pid: %d, filho: %d\n",i+1, getpid(), pid);
    }
    wait(NULL);
  return 0;
}
