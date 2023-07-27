#include <unistd.h>
#include <stdio.h>
#include <signal.h>

int segundos = 0;
int ctrl = 0;

void handler(int signumber){
  switch(signumber){
      case SIGALRM:
        segundos++;
        alarm(1);
        break;
      case SIGINT:
        ctrl++;
        printf("Tempo decorrido: %d", segundos);
        break;
      case SIGQUIT:
        printf("ctrl+c: %d", ctrl);
        _exit(0);
  }
}

int main(int argc, char *argv[]){

  signal(SIGALRM, handler);
  signal(SIGINT, handler);
  signal(SIGQUIT, handler);

  alarm(1);
  while(1){
    pause();
  }
  return 0;
}
