/*Scrivere un programma che generi un file, usando i descriptor, al posto di stdout, 
che fa scrivere a N figli, l'ouput di vari comandi linux su questo file, infine 
il padre stamperà il contenuto del file nell'output standard*/
#include <unistd.h> 
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
  const char *fileName = "prova00.txt";
  if(argc == 1)
  {
    printf("Usage: ./00.o <comando> ... <comando>");
    exit(1);
  }
  system("rm prova00.txt");
  for(int i = 1; i < argc; i++)
  {
    int son = fork();
    if(son == 0){
      // printf("Esecuzione figlo %s\n", argv[i]);
      char *sonArgv[] = {argv[i], NULL};
      execv("./write.o", sonArgv);
      exit(getpid());
    }
  }
  while(wait(NULL)>0);
  // printf("Esecuzione dei figli completa\n");
  int bytesRead;
  int size = 30;
  char buf[30];
  int input = open(fileName, O_RDONLY);
  do{
    bytesRead = read(input, buf, size);
    buf[bytesRead] = 0;
    printf("%s", buf);
  }while(bytesRead > 0);
  close(input);
  return 0;
}

