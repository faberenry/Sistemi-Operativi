#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define DIM 50
#define WRITE 1
#define READ 0

int main(int argc, char const *argv[])
{
  int pipeFather[2], pipeSon[2];
  char buffer[DIM];
  int esito = pipe(pipeFather);
  int esito1 = pipe(pipeSon);
  if(esito == 0 && esito1 == 0){
    int son = fork();
    if(son == 0){
      //figlio: scrive - legge
      close(pipeFather[WRITE]); //lato scrittura verso padre
      close(pipeSon[READ]); //lato lettura mia pipe
      write(pipeSon[WRITE], "Sono figlio\n", 14);
      close(pipeSon[WRITE]);
      int readSon = read(pipeFather[READ], &buffer, DIM); //leggo quello scritto da padre
      close(pipeFather[READ]);
      printf("Figlio ha letto: %s\n", buffer);
    }else{
      //padre: legge - scrive
      close(pipeFather[READ]); //chiudo lato lettura mia pipe
      close(pipeSon[WRITE]); //lato scrittura verso figlio
      int readFather = read(pipeSon[READ], &buffer, DIM);
      close(pipeSon[READ]);
      printf("Padre ha letto: %s\n", buffer);
      //scrivo su pipe figlio
      write(pipeFather[WRITE], "Luke sono una marionetta\n", 27);
      close(pipeFather[WRITE]);
    }
    while(wait(NULL)>0);
  }else{
    printf("Errore creazione pipe\n");
  }
  return 0;
}

/*ricordarsi che si scrive su sè stessi e si legge sulla pipe dell'altro processo*/