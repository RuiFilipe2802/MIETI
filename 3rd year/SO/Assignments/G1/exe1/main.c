#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char* argv[]) {

	if(argc !=3){
		fprintf(stderr, "utilizacao: %s origem destino\n", argv[0]);
		return 1;
	}
	int fdi = open(argv[1], O_RDONLY);
	if(fdi == -1){
		perror(argv[1]);
		return 1;    //Insucesso
	}
	int fdo = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC , 0666);
	if(fdo == -1){
		perror(argv[2]);
		close(fdi);
		return 1;
	}

	int n;
	char buffer[1024];   //buf size
	while((n=read(fdi, buffer, sizeof(buffer)))>0){   // enquanto estiver a ler é >0
		write(fdo, buffer, n);
	}
	close(fdi);
	close(fdo);
	return 0;   // Retornar sucesso
}
