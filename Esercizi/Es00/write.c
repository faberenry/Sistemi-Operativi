#include <unistd.h> 
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
  const char *fileName = "prova00.txt";
  int output = open(fileName, O_APPEND|O_RDWR|O_CREAT, S_IRUSR | S_IWUSR);
  dup2(output, 1);
  dup2(output, 2);
  system(argv[0]);
  close(output);
  return 0;
}
