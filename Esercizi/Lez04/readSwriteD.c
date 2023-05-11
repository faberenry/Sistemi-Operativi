//leggo un file con gli streams e lo riscrivo su un altro con i descripto
//file di lettura "prova.txt"
#include <unistd.h> 
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#define DIM 100

int main()
{
  char reading[DIM];
  int output = open("scrittura.txt", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
  FILE *input = fopen("prova.txt", "r");
  if(input != NULL)
  {
    printf("File input aperto correttamente\n");
  }
  printf("File output aperto: %d", output);
  if(input != NULL)
  {
    while(!feof(input))
    {
      fgets(reading, 10, input);
      reading[10] = '\0';
      printf("Ho letto: %s\n", reading);
      write(output, reading, strlen(reading));
      // printf("Status operation = %d\n", status);
    }
  }
  fclose(input);
  close(output);
  return 0;
}

/*
int main(int argc, char const *argv[])
{
  // creat("scrittura.txt");
  int output = open("scrittura.txt", O_RDWR|O_CREAT);
  write(output, "ciao\n", strlen("ciao\n "));
  close(output);
  return 0;
}*/
