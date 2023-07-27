#include <sys/types.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

ssize_t readln(int fd, char *line, size_t size){

	static char buffer[1024];
  static int buffer_n = 0;
  int i=0;
  int n;

	while(i < size && (n = read(fd, buffer, sizeof(buffer)) == 1) && line[i] != '\n'){
    i++;
  }

  if (n == -1) return i > 0 ? i : -1;
  return n == 0 ? i : i+1;
}

int main(int argc, char* argv[]){

	char buf[8];
  int r;
  r = readln(0, buf, sizeof(buf));

}
