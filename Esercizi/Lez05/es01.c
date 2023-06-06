/*Scrivere un programma in C che avendo come argomenti dei “binari”, 
  si eseguono con exec ciascuno in un sottoprocesso*/
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <time.h> 
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
  if(argc == 1){
    printf("Usage: <nomeFile> <binary1> <binary2> ... <binaryN>\n");
  } else {
    for(int i = 1; i < argc; i++)
  {
    int sonPid = fork();
    if(sonPid == 0){
      //faccio exec con l'elemento che mi viene passato
      // printf("Pid process: %d\n", getpid());
      fflush(stdout);
      char par[20];
      snprintf(par, 10, "par_%d", getpid());
      char *sonArgv[] = {par, NULL};
      execv(argv[i], sonArgv);
      exit(getpid());
    }
  }
  while(wait(NULL) > 0);
  printf("Sono padre con pid %d e aspetto i miei figli\n", getpid());
  }
  return 0;
}
