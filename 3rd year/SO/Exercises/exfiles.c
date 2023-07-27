#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){

  int i;
  char buffer[20];
  int fdinput = open(argv[1], O_RDONLY);
  int fdoutput = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);

  if(fdinput == -1){
    perror(argv[1]);
    return 1;
  }
  if(fdoutput == -1){
    perror(argv[2]);
    return 1;
  }

  while((i = read(fdinput, buffer, sizeof(buffer))) > 0){
    printf("%d\n", i);
    write(fdoutput, buffer, i);
  }

  close(fdinput);
  close(fdoutput);
  return 0;

}
