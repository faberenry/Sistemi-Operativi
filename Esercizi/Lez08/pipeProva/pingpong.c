//creo ping-pong infinito tramite pipeAnonime
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define WRITE 1
#define READ 0
#define DIM_BUFFER 50

int main(int argc, char const *argv[])
{
  int pipePing[2], pipePong[2];
  char buffer[DIM_BUFFER];
  int esito = pipe(pipePing);
  int esito1 = pipe(pipePong);
  if(esito == 0 && esito1 == 0){
    int son = fork();
    if(son == 0){
      while(1){
        //figlio: scrive - legge
        close(pipePing[WRITE]); //lato scrittura verso padre
        close(pipePong[READ]); //lato lettura mia pipe
        write(pipePong[WRITE], "Ping\n", strlen("Ping\n"));
        close(pipePong[WRITE]);
        int readSon = read(pipePing[READ], &buffer, DIM_BUFFER); //leggo quello scritto da padre
        close(pipePing[READ]);
        printf("Son: %s\n", buffer);
        sleep(2);
      }
    }else{
      while(1){
        //padre: legge - scrive
        close(pipePing[READ]); //chiudo lato lettura mia pipe
        close(pipePong[WRITE]); //lato scrittura verso figlio
        int readFather = read(pipePong[READ], &buffer, DIM_BUFFER);
        close(pipePong[READ]);
        printf("Father: %s\n", buffer);
        //scrivo su pipe figlio
        write(pipePing[WRITE], "Pong\n", 27);
        close(pipePing[WRITE]);
        sleep(2);
      }
    }
    while(wait(NULL)>0);
  }else{
    printf("Errore creazione pipe\n");
  }
  return 0;
}
