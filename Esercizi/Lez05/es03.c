/*
Dati due eseguibili come argomenti del tipo ls e wc si eseguono in due processi distinti: il primo 
deve generare uno stdout redirezionato su un file temporaneo, mentre il secondo deve essere 
lanciato solo quando il primo ha finito leggendo lo stesso file come stdin.
Ad esempio ./main ls wc deve avere lo stesso effetto di ls | wc.
*/
#include <unistd.h> 
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
  char buf[21];
  char *fileName = "esercizio03.txt";
  if(argc != 3)
  {
    printf("Usage: ./es03.o <comando> <comando>\n");
    return 1;
  }
  int son = fork();
  if(son == 0){
    //figlio esegue primo coma ndo
    FILE *output = freopen(fileName, "w", stdout);
    // printf("ciao sono figlio %d\n", getpid());
    snprintf(buf, 20, "%s", argv[1]);
    system(buf);
    fclose(output);
    exit(getpid());
  }else{
    //padre che aspetta ed esegue il secondo comando
    wait(&son);
    FILE *input = freopen(fileName, "r", stdin);
    snprintf(buf, 20, "%s", argv[2]);
    system(buf);
    fclose(input);
  }
  return 0;
}
