#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
  int i;
  int fd1[2];
  int fd2[2];
  if(pipe(fd1) == -1){
    perror("Erro na abertura do pipe 1");
    return 1;
  }
  if(pipe(fd2) == -1){
    perror("Erro na abertura do pipe 2");
    return 1;
  }
  int pid = fork();
  int x;
  int x_received;
  switch(pid){
      case -1:
        perror("Erro no fork");
        return 1;

      case 0:
        close(fd2[0]);
        close(fd1[1]);
        int number;
        read(fd1[0], &number, sizeof(number));
        int number2 = number * 4;
        printf("%d",number);
        write(fd2[1], &number2, sizeof(number2));
        close(fd2[1]);
        close(fd1[0]);
        _exit(1);

      default:
        close(fd2[1]);
        close(fd1[0]);
        printf("Escoha um numero entre 0 e 9\n");
        scanf("%d",&x);
        write(fd1[1], &x, sizeof(x));
        read(fd2[0], &x_received, sizeof(x_received));
        write(1, &x_received, sizeof(x_received));
        close(fd2[0]);
        close(fd1[1]);
        wait(NULL);
  }
}
