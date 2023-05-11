#include <stdio.h>
#include <string.h>
#include <stdlib.h>


//non funziona e non ho vogliia di capire perchè
char* stringRev(char * str)
{
  int length = strlen(str);
  char* rev =  (char*) malloc((length+1)*sizeof(char));
  for(int i = length; i > 0; i--)
  {
    printf("Elemento di str in pos %d: %c\n", i-1, str[i-1]);
    printf("Copio in rev in pos %d\n", (length-i));
    // *(rev+(length-i)) = str[i-1];
    rev[length-i] = str[i-1];
  }
  rev[length+1] = '\0';
  return *rev;
}

int stringPos(char *str, char chr)
{
  int found = 0;
  for(int i = 0; str[i] != '\0' && found == 0; i++)
  {
    if(str[i] == chr)
    {
      found = i;
    }
  }
  return found;
}

int main(int argc, char ** argv[]){
  int pos;
  char c = 'a';
  char *str = "ciao";
  char *rev;
  pos = stringPos(str, c);
  rev = stringRev(str);
  printf("Posizione: %d \n", pos);
  printf("Reverse: %s \n", rev);
  return 0;
}