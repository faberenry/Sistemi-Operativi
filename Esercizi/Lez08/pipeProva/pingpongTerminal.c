//creo ping-pong infinito tramite pipeAnonime
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define WRITE 1
#define READ 0
#define DIM_BUFFER 50

int main(int argc, char const *argv[])
{
  char *endConversation = "fine";
  int pipePing[2], pipePong[2];
  char bufferSon[DIM_BUFFER];
  char bufferFather[DIM_BUFFER];
  int esito = pipe(pipePing);
  int esito1 = pipe(pipePong);
  int messageCount = atoi(argv[1]);
  if(esito == 0 && esito1 == 0){
    int son = fork();
    if(son == 0){
      do{
        //figlio: scrive - legge
        close(pipePing[WRITE]); //lato scrittura verso padre
        close(pipePong[READ]); //lato lettura mia pipe
        //write(pipePong[WRITE], "Ping", strlen("Ping"));
        close(pipePong[WRITE]);
        int readSon = read(pipePing[READ], &bufferSon, DIM_BUFFER); //leggo quello scritto da padre
        close(pipePing[READ]);
        printf("Son: %s\n", bufferSon);
        sleep(2);
      }while(strcmp(bufferSon, endConversation) != 0);
      exit(1);
    }else{
      char *strPong = "Pong";
      for(int i = 0; i < messageCount; i++){
        if(i == messageCount-1){
          printf("Ho già scritto %d volte, chiudo la conversazione\n", i);
          // strcpy(strPong, endConversation); non funziona perchè la lunghezza di strPong è minore di endConversation
          // printf("valueOf(strPong) = %s\n", strPong);
        }
        //padre: legge - scrive
        close(pipePing[READ]); //chiudo lato lettura mia pipe
        close(pipePong[WRITE]); //lato scrittura verso figlio
        int readFather = read(pipePong[READ], &bufferFather, DIM_BUFFER);
        close(pipePong[READ]);
        printf("Father: %s\n", bufferFather);
        //scrivo su pipe figlio
        if(i < messageCount-1) printf("Scrivo strpong = %s\n", strPong);
        else printf("Scrivo endConversation: %s\n", endConversation);
        if(i <= messageCount-1) {write(pipePing[WRITE], strPong, strlen(strPong));}
        else write(pipePing[WRITE], endConversation, strlen(endConversation));
        close(pipePing[WRITE]);
        sleep(2);
      }
    }
    while(wait(NULL)>0);
    printf("Conersazione finita\n");
  }else{
    printf("Errore creazione pipe\n");
  }
  return 0;
}
