#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char const *argv[]) {

  int n;
  char buffer[20];

  while((n = read(0, buffer, sizeof(buffer))) > 0){
    write(1, buffer, n);
  }
  
  return 0;
}
