#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define DIM 50
#define WRITE 1
#define READ 0

int main(int argc, char const *argv[])
{
  int pipeWriter[2];
  char buffer[DIM];
  int esito = pipe(pipeWriter);
  if(esito == 0){
    int son = fork();
    if(son == 0){
      //figlio = writer
      close(pipeWriter[READ]); //chiudo lettura della pipe in modo da non far leggere il padre
      //scrivo su pipe 
      write(pipeWriter[WRITE], "Sono figlio\n", 13);
      close(pipeWriter[WRITE]); //chiudo la scrittura
    }else{
      //padre = reader
      close(pipeWriter[WRITE]); //chiudo scrittura per leggere
      int r = read(pipeWriter[READ], &buffer, DIM);
      close(pipeWriter[READ]);
      printf("Messaggio letto: %s\n", buffer);
    }
    while(wait(NULL)>0);
  }else{
    printf("Errore creazione pipe\n");
  }
  return 0;
}
