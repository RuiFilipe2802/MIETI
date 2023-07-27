#include <sys/types.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int readln(int fd, char *line, int size){

	int i = 0;
	char buf[1];
	int n;
	while( i < size && (n = read(fd, &line[i], 1)) > 0 && line[i] != '\n'){
		i++;
	}

	if (n == -1) return i > 0 ? i : -1;   // erro na leitura
	return n == 0 ? i : i+1;
}

int main(int argc, char *argv[]){
		char buffer[1024];
		int fdi = open(argv[1], O_RDONLY);
		int r;
		r = readln(fdi, buffer, sizeof(buffer));
		write(1, buffer, r);
		return 0;
}
