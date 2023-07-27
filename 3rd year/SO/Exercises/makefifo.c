#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){

  execlp("mkfifo","mkfifo", "fifo", NULL);
  return 1;
}
