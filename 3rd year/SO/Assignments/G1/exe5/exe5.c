#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <assert.h>
#include <string.h>

ssize_t readln(int fd, char *line, size_t size){
	int i = 0;
	int n;
	while( i < size && (n = read(fd, &line[i], 1) == 1) && line[i] != '\n'){
		i++;
	}
	if (n == -1) return i > 0 ? i : -1;   // erro na leitura
	return n == 0 ? i : i+1;
}

int main(int argc, char *argv[]){
	assert(argc == 2);
	int fd = open(argv[1], O_RDONLY);
	assert(fd != -1);

	char buffer[1024];
	int n, linhas = 1;
	while(1){
		snprintf(buffer, 8, "%6d ", linhas);
		n = readln(fd, &buffer[7], sizeof(buffer)-7);
		if(n <= 0){
			break;
		}
		write(1, buffer, n+7);

		linhas++;
	}
	close(fd);
	return 0;
}
