#include <stdio.h> //gestione I/O
#include <string.h> //gestione stringe
#include <stdlib.h> //strcmp
#define TRUE 1 //define TRUE value
#define FALSE 0 //define FALSE value
#define MAX_PARAMS 4 //define maximum parametes
typedef char bool;
const char * valid[] = {"-h","-m","-n","--help"};

bool isValid(char *arg)
{
  for (int i = 0; i < MAX_PARAMS; i++)
  {
    if(!strcmp(arg, valid[i]))
    {
      return TRUE;
    }
  }
  return FALSE;
}

int main(int argc, char *argv[])
{
  char *list[MAX_PARAMS];
  int index = 0;
  for (int i = 1; i < argc; i++)
  {
    if(isValid(argv[i]))
    {
      list[index] = argv[i];
      index++;
    }else
    {
      fprintf(stderr, "%s in not valid\n", argv[i]);
      exit(2);
    }
  }
  for(int i = 0; i < index; i++)
  {
    printf("%s ", list[i]);
  }
  printf("\n");
  return 0;
}