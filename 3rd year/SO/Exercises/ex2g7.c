#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX 100

// /multigrep palavra fich1 fich2 ... fichN

int  main(int argc, char *argv[]){

  int nProc = argc-2;
  char *palavra = argv[1];
  int em_execucao = 0;
  int status;
  pid_t pids[MAX];

  for(int i=0;i!=nProc;i++){
    pids[em_execucao] = fork();
    em_execucao++;
    if(pids[em_execucao-1] == 0){   //FILHO
      execlp("grep", "grep", palavra, argv[i+2], NULL);
      perror("GREP");
      _exit(i+1);
    }
  }
  while(em_execucao!=0){
    int i;
    pid_t pid = wait(&status);
    for(i = 0; i!= nProc && pids[i] != pid;i++);
    pids[i] = 0;
    if(WIFEXITED(status) && WEXITSTATUS(status) == 0){

      for(i=0; i!= nProc; i++){
        if(pids[i] != 0){
          kill(SIGKILL, pids[i]);
        }
      }
    }
    em_execucao--;
  }
  return 0;
}
