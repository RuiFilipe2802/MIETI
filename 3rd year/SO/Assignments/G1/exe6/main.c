#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <assert.h>
#include <string.h>

typedef struct Pessoa{
  char nome[128];
  int idade;

}pessoa;

int procura(int f, char* nome){
  struct Pessoa p;
  int n;
  lseek(0, &f, SEEK_SET);
  while((n == read(f, &p, sizeof(p))) == sizeof(p) && strcmp(p.nome, nome) != 0){
    return n <= 0  ? -1 : p.idade; // retorna idade caso contrario -1
  }
}

int main(int argc, char* argv[]){

  struct Pessoa p;
  //struct Pessoa pessoa2;

  int d = open("pessoas.db", O_CREAT | O_RDWR, 0666);
  int n;
  if (d == -1){
    perror("pessoas.db");
    return 1;
  }

  if(argc == 1){
    while(read(d, &p, sizeof(p)) == sizeof(p)){
      char buffer[128*2];
      snprintf(buffer, sizeof(buffer), "%s, %d\n", p.nome, p.idade);
      write(1, buffer, strlen(buffer));
    }
  }


  else
  if(argc == 4 && strcmp(argv[1], "-i") == 0){
    if(procura(d, argv[2]) != -1){
      fprintf(stderr,"registo ja existente\n");
      close(d);
      return 1;

    }
    strcpy(p.nome, argv[2]);
    p.idade = atoi(argv[3]);
    //lseek(d, 0, SEEK_END);    ja nao é necessario devido à procura
    write(d, &p, sizeof(p));
  }
  else
  if(argc == 4 && strcmp(argv[1], "-u") == 0){
    if(procura(d, argv[2]) != -1){
      fprintf(stderr, "registo inexistente\n");
      close(d);
      return 1;
    }
    strcpy(p.nome, argv[2]);
    p.idade = atoi(argv[3]);
    lseek(d, -sizeof(p), SEEK_CUR);   // Recuar um registo
    write(d, &p, sizeof(p));
    //lseek(d, 0, SEEK_END);    ja nao é necessario devido à procura
    write(d, &p, sizeof(p));
  }

  close(d);
  return 0;
}
