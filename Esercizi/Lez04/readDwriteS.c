//leggo un file con gli streams e lo riscrivo su un altro con i descripto
//file di lettura "prova.txt"
#include <unistd.h> 
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#define DIM 100

int main(int argc, char const *argv[])
{
  char buf[DIM];
  int bytesRead = -1;
  FILE *output = fopen("scritturaConStream.txt", "w");
  int input = open("prova.txt", O_RDONLY);
  if(input > 0 && output != NULL) 
  {
    do{
      bytesRead = read(input, buf, 10); //numeor di che legge -1, ricordarsi di mettere in pos byteRead elemento di fine stringa
      // printf("Letto: %s\n", buf);
      // printf("BytesToRead = %d\n", bytesRead);
      buf[bytesRead] = '\0';
      fputs(buf, output);
    }while(bytesRead > 0);
  }
  close(input);
  fclose(output);
  return 0;
}
