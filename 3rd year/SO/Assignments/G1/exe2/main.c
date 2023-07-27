#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]){

	char c[1024];
	int n;
	while((n=read(0, c, sizeof(c))) > 0) { 		//   0 == STDIN_FILENO
		write(1, c, n);											 		//   1 == STDOUT_FILENO
	}
	return 0;
}
