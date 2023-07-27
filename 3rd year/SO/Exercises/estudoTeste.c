#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]) {

  int n, i;
  char buffer[256];
  char *args[100];
  char *p = NULL;

  int fd = open(argv[1], O_RDONLY);
  /*if(fd == -1){
    perror(argv[1]);
    return 1;
  }*/
  while((n = read(fd, buffer, sizeof(buffer))) > 0){
    for(p = strtok(buffer, " "); p != NULL; p = strtok(NULL, " ")){
      args[i] = strdup(p);
      i++;
    }
    args[i] = NULL;
    execvp("ls", args);
  }
  close(fd);
  return 0;
}
