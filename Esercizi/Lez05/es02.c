#include <unistd.h> 
#include <stdio.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
  FILE *output = freopen("esercizio02.txt", "a+", stdout);
  fprintf(stdout, "Sono output %s\n", argv[0]);
  fflush(stdout);
  fclose(output);
  return 0;
}
