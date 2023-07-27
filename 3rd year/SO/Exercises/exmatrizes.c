#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char const *argv[]) {

  int matriz[2][3];
  int i;
  int j;
  int fd = open(argv[1], O_RDWR | O_CREAT, 0666);

  for(i=0;i<2;i++){
    for(j=0;j<3;j++){
      matriz[i][j]=rand() % 50;
      //write(fd, matriz, sizeof(matriz));
    }
  }

  write(fd, matriz, sizeof(matriz));
  //printf("%d", matriz);

  return 0;
}
