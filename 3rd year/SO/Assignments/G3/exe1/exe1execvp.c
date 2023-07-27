#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
  char* args[] = {"ls", "-l", NULL};
  execvp("ls", args);
  perror("ls");
  return 1;
}
