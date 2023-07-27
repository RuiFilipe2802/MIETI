#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define MAX 50

//    SERVIDOR

void handlerAlarm(int sig)
{
  kill(getpid(), SIGKILL);
}

int readln(int fd, char *line, int size)
{
  int i = 0, n;
  while (i < size && (n = read(fd, &line[i], 1)) == 1 && line[i] != '\n')
  {
    i++;
  }
  return i;
}

int main(int argc, char *argv[])
{

  char *arrFilters[10][MAX]; // ARRAY WITH FILTERS
  char *arrExec[10][MAX];    // ARRAY WITH FILTER EXECUTABLES
  int arrN[10];              // ARRAY WITH NUMBER OF FILTERS
  int filters, exe, num, numFilters;
  write(1, "WELCOME\n", 9);
  mkfifo("fifoC_S", 0666); // CLIENTE -> SERVIDOR
  mkfifo("fifoS_C", 0666); // SERVIDOR -> CLIENTE

  char path[150] = "../";
  strcat(path, argv[2]);
  char filtersDestination[13] = "../";
  strcat(filtersDestination, argv[1]);
  int configFile = open(filtersDestination, O_RDONLY);
  char buf[25];
  int sizeBuf;
  int nProc = 0;
  int nLine = 0;
  int fd;

  char filtersFromClient[10][MAX];

  if (argc == 1)
  { //     Show usage protocol
    write(1, "./aurrasd config-filename filters-folder\n", 42);
    return 1;
  }

  while ((sizeBuf = readln(configFile, buf, sizeof(buf))) > 0)
  {

    if ((nLine % 3) == 0)
    { // FILTER
      arrFilters[filters][MAX] = malloc(sizeof(buf));
      strncpy(arrFilters[filters][MAX], buf, sizeBuf);
      filters++;
    }

    if ((nLine % 3) == 1)
    { // FILTER EXECUTABLE
      arrExec[exe][MAX] = malloc(sizeof(buf));
      strncpy(arrExec[exe][MAX], buf, sizeBuf);
      exe++;
    }

    if ((nLine % 3) == 2)
    { // NUMBER OF FILTERS
      numFilters = atoi(buf);
      arrN[num] = numFilters;
      nProc = nProc + numFilters;
      num++;
    }
    nLine++;
  }

  int r, m;
  int size;
  int args;
  char *argums[50];
  char *p = NULL;
  int j;
  int file_input;
  int file_output;
  int pidPro = 0;

  char bufferState[100];

  signal(SIGALRM, handlerAlarm);

  fd = open("fifoC_S", O_RDONLY);
  while (1)
  {

    while (read(fd, &args, sizeof(int)) > 0)
    {

      // GET NUMBER OF ARGUMENTS
      read(fd, &size, sizeof(int)); // GET SIZE MESSAGE
      char buffer[size];
      read(fd, buffer, size); // GET MESSAGE
      int i = 0;
      int status;
      char iF[50] = "../";
      char obJ[50]= "../obj/";
      char path2[150];
      strcpy(path2, path);
      if (strcmp(buffer, "status") == 0)
      {
        int fd2 = open("fifoS_C", O_WRONLY);
        write(fd2, bufferState, 50);
        for (size_t x = 0; x < 5; x++)
        {
          write(fd2, &arrN[x], sizeof(int));
        }
        write(fd2, &pidPro, sizeof(int));
        break;
      }
      strcpy(bufferState, buffer);
      for (p = strtok(buffer, " \n"); p != NULL; p = strtok(NULL, " "))
      {
        argums[i] = strdup(p);
        i++;
      }
      char *inputFile, *outputFile;
      inputFile = argums[0];
      strcat(iF, inputFile);
      outputFile = argums[1];
      strcat(obJ,outputFile);
      for (size_t k = 2; k < args; k++)
      {
        char path2[150];
        strcpy(path2, path);
        if (strcmp(argums[k], arrFilters[0][MAX]) == 0 && arrN[0] != 0)
        {
          strcat(path2, arrExec[0][MAX]);
          arrN[0]--;
          int pid = fork();
          switch (pid)
          {
          case -1: 
            perror("fork");
          case 0:
            file_input = open(iF, O_RDONLY);
            file_output = open(obJ, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            dup2(file_input, 0);
            close(file_input);
            dup2(file_output, 1);
            close(file_output);
            alarm(15);
            execl(path2, path2, NULL);
            _exit(1);
          default: 
            printf("Processo inicializado = %d\n", pid);
          }
          pid = wait(&status);
          pidPro = pid;
          printf("Filtro executado com sucesso = %d\n", pid);
          arrN[0]++;
          if (WIFEXITED(status))
          {
            printf("exit: %d\n", WEXITSTATUS(status));
          }
          else
          {
            printf("Erro ao executar o filtro\n");
          }
        }
        if (strcmp(argums[k], arrFilters[1][MAX]) == 0 && arrN[1] != 0)
        {
          strcat(path2, arrExec[1][MAX]);
          arrN[1]--;
          int pid = fork();
          switch (pid)
          {
          case -1: 
            perror("fork");
          case 0: 
            file_input = open(iF, O_RDONLY);
            file_output = open(obJ, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            dup2(file_input, 0);
            close(file_input);
            dup2(file_output, 1);
            close(file_output);
            alarm(15);
            execl(path2, path2, NULL);
            _exit(1); 
          default:    
            printf("Processo inicializado = %d\n", pid);
          }
          pid = wait(&status);
          pidPro = pid;
          printf("Filtro executado com sucesso = %d\n", pid);
          arrN[1]++;
          if (WIFEXITED(status))
          {
            printf("exit: %d\n", WEXITSTATUS(status));
          }
          else
          {
            printf("Erro ao executar o filtro\n");
          }
        }
        if (strcmp(argums[k], arrFilters[2][MAX]) == 0 && arrN[2] != 0)
        {
          strcat(path2, arrExec[2][MAX]);
          arrN[2]--;
          int pid = fork();
          switch (pid)
          {
          case -1: 
            perror("fork");
          case 0: 
            file_input = open(iF, O_RDONLY);
            file_output = open(obJ, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            dup2(file_input, 0);
            close(file_input);
            dup2(file_output, 1);
            close(file_output);
            alarm(15);
            execl(path2, path2, NULL);

            _exit(1); 
          default:   
            printf("Processo inicializado = %d\n", pid);
          }
          pid = wait(&status);
          pidPro = pid;
          printf("Filtro executado com sucesso = %d\n", pid);
          arrN[2]++;
          if (WIFEXITED(status))
          {
            printf("exit: %d\n", WEXITSTATUS(status));
          }
          else
          {
            printf("Erro ao executar o filtro\n");
          }
        }
        if (strcmp(argums[k], arrFilters[3][MAX]) == 0 && arrN[3] != 0)
        {
          strcat(path2, arrExec[3][MAX]);
          arrN[3]--;
          int pid = fork();
          switch (pid)
          {
          case -1: 
            perror("fork");
          case 0: 
            file_input = open(iF, O_RDONLY);
            file_output = open(obJ, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            dup2(file_input, 0);
            close(file_input);
            dup2(file_output, 1);
            close(file_output);
            alarm(15);
            execl(path2, path2, NULL);

            _exit(1); 
          default:   
            printf("Processo inicializado = %d\n", pid);
          }
          pid = wait(&status);
          pidPro = pid;
          printf("Filtro executado com sucesso = %d\n", pid);
          arrN[3]++;
          if (WIFEXITED(status))
          {
            printf("exit: %d\n", WEXITSTATUS(status));
          }
          else
          {
            printf("Erro ao executar o filtro\n");
          }
        }
        if (strcmp(argums[k], arrFilters[4][MAX]) == 0 && arrN[4] != 0)
        {
          strcat(path2, arrExec[4][MAX]);
          arrN[4]--;
          int pid = fork();
          switch (pid)
          {
          case -1: 
            perror("fork");
          case 0: 
            file_input = open(iF, O_RDONLY);
            file_output = open(obJ, O_WRONLY | O_CREAT, 0666);
            dup2(file_input, 0);
            close(file_input);
            dup2(file_output, 1);
            close(file_output);
            alarm(15);
            execl(path2, path2, NULL);

            _exit(1); 
          default:   
            printf("Processo inicializado = %d\n", pid);
          }
          pid = wait(&status);
          pidPro = pid;
          printf("Filtro executado com sucesso = %d\n", pid);
          arrN[4]++;
          if (WIFEXITED(status))
          {
            printf("exit: %d\n", WEXITSTATUS(status));
          }
          else
          {
            printf("Erro ao executar o filtro\n");
          }
        }
      }
      strcpy(inputFile, obJ);
      sleep(1);
    }
  }

  return 0;
}
