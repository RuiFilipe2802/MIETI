#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char const *argv[]) {

  int fd = open(argv[1], O_RDONLY);
  int n;
  char buffer[100];
  char *args[];

  if(fd == -1){
    perror("couldnt open files");
    return 1;
  }

  while ((n = read(fd, buffer, sizeof(buffer))) >0) {

  }
  close(fd);

  return 0;
}


/*
  pid_t pid;
  int i, status;
  for(i=0;i!=5;i++){
    pid = fork();
    switch (pid) {
      case -1:
        perror("fork");
        return 1;
      case 0:
        printf("id: %d, pid do filho: %d, pid do pai: %d\n", i+1, getpid(), getppid());
        _exit(i+1);
      default:
        printf("sou o pai do filho: %d\n", pid);
        pid = wait(&status);
        if(WIFEXITED(status)){
          printf("exit: %d\n", WEXITSTATUS(status));
        }

    }
  }

 for(i=0;i!=5;i++){
    pid = wait(&status);
    if(WIFEXITED(status)){
      printf("exit: %d\n", WEXITSTATUS(status));
    }
  }*/
