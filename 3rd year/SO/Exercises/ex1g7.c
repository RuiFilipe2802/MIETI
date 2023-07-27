#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>

int segundos = 0;
int ctrlc = 0;

void handler(int signumber){
  switch(signumber){
    case SIGALRM:
      segundos++;
      alarm(1);
      break;

    case SIGINT:
      ctrlc++;
      printf("Tempo: %d segundos\n", segundos);
      break;

    case SIGQUIT:
      printf("Carregou %d vezes em ctrl+c", ctrlc);
      _exit(0);
  }
}

int main(int argc, char *argv[]){

  signal(SIGALRM, handler);
  signal(SIGINT, handler);
  signal(SIGQUIT, handler);
  alarm(1);
  pause();
  return 0;

}
