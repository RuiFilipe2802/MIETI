#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#define MAX 128

int main(int argc, char const *argv){

  int n;
  char buf[MAX];

  int fd3 = open("erros.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
  dup2(fd3,2); close(fd3);

  int fd = open("/etc/passwd", O_RDONLY);
  if(fd == -1){
    perror("Erro no passwd");
    return -1;
  }
  dup2(fd,0); close(fd);

  int fd2 = open("saida.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
  dup2(fd2,1); close(fd2);

  if(!fork()){
    n = read(0,buf,sizeof(buf));
    write(1,buf,n);
  }

  return 0;
}
