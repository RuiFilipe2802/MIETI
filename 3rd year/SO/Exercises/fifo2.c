#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){

  int f = open("fifo", O_RDONLY);
  char buffer[8];
  int r = read(f, buffer, sizeof(buffer));
  write(1, buffer, r);

  return 0;
}
