#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){

	int file;
	char c[13] = "Hello World!\n";

	file = open(argv[1], O_RDWR | O_CREAT, 0666);

	if(file == -1){
		perror(argv[1]);
		exit(1);
	}

	write(file, &c, 13);
	//read(file, &c, 15);
	close(file);
	printf("buf: %s", c);

	return 0;

}
