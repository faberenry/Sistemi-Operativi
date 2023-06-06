#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[])
{
  char *fifoName = "/tmp/sommaFifo";
  int fd;
  char buffer[10];
  memset(buffer, 0, 10);
  if((argc != 2))
  {
    printf("Usage: ./somma.o <N>\n");
    return 1;
  }
  int numFigli = atoi(argv[1]); //conversione da char to int
  mkfifo(fifoName, S_IRUSR|S_IWUSR);
  // perror("Pipe creata?");
  printf("Genero %d figli\n", numFigli);
  for(int i = 0; i < numFigli; i++){
    int son = fork();
    if(son == 0){
      srand(time(NULL));
      printf("Pid: %d\n", getpid());
      int randomNumber = (rand()%10); // genero numero da 0 a 9
      printf("Generato: %d\n", randomNumber);
      buffer[0] = '0'+randomNumber; //converto to char e lo butto nell pos 0 del buffer
      buffer[1] = '\0'; 
      printf("Valore nel buffer %s\n", buffer);
      int fileDescriptor = open(fifoName, O_APPEND); //apro in appendo in modo da non cancellare 
      //quello che hanno scritto gli altri figli
      printf("Valore fileDescriptor: %d\n", fileDescriptor);
      if(fileDescriptor >= 0){
        write(fileDescriptor, buffer, 3); 
        close(fileDescriptor);
        printf("Sono %d, ho scritto %s sulla fifo %s\n", getpid(), buffer, fifoName);
      }else{
        printf("Errore apertura fifo\n");
        exit(1);
      }
      printf("Pid: %d\n", getpid());
      exit(1);
    }
  }
  while(wait(NULL)>0); //aspetto che tutti i figli abbiano scritto su pipe
  printf("Ho aspettato tutti i figli\n");
  return 0;
}


/*srand(time(NULL));
      //genero numero random su figlio e scrivo su pipe
      int random = 1;
      fd = open(fifoName, O_APPEND); //apro in append
      if(fd < 0){
        fprintf(stderr, "%s: Failed to open FIFO %s\n", argv[argc-argc], fifoName);
        exit(random);
      }else{
        fprintf(stdout, "Fd value = %d\n", fd);
      }
      random = (int)(rand()%10);
      printf("Generato: %d\n", random);
      buffer[0] = random+'0';
      buffer[1] = '\0';
      //printf("Sono figlio %d, scrivo %s su pipe\n", getpid(), buffer);
      write(fd, buffer, 3);
      close(fd);
      exit(random);*/

/*
 //leggo pipe e calcolo la somma dei valori
  int bytesRead = -1;
  int sum = 0;
  fd = open(fifoName, O_RDONLY|S_IRUSR|S_IWUSR);
  // printf("File Descriptor: %d\n", fd);
  if(fd > 0){
    do{
      bytesRead = read(fd, buffer, 9);
      buffer[bytesRead] = '\0';
      printf("Ho letto %s, converto\n", buffer);
      int valueRead = atoi(buffer);
      printf("Sono padre ho letto valore: %d\n", valueRead);
      sum += valueRead;
      sleep(1);
    }while(bytesRead > 0);
    close(fd);
    printf("Somma dei valori: %d\n", sum);
  }else{
    fprintf(stderr, "%s: Failed to open FIFO %s\n", argv[argc-argc], fifoName);
    return 1;
  }*/
