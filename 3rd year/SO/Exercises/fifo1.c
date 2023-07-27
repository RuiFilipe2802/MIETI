#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){

  int x = open("fifo", O_WRONLY);
  char buffer[8];
  int d = read(0, buffer, sizeof(buffer));
  write(x, buffer, sizeof(buffer));

  return 0;
}
