/*NON FUNZIONA CON LE FIFO, BELLA MERDA*/

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define READ 0 
#define WRITE 1

int main(int argc, char const *argv[])
{
  printf("Processo padre: %d\n", getpid());
  char *fifoName = "/tmp/sommafifo";
  mkfifo(fifoName, 0755);
  int son, rNum;
  int fd;
  fd = open(fifoName, O_WRONLY|O_TRUNC|O_CREAT, 0755);
  if(fd == -1){
    printf("Errore fd\n");
    return 1;
  }
  close(fd);
  if(argc != 2){
    printf("Wrong input\n");
    return 2;
  }
  int numFigli = atoi(argv[1]);
  printf("NumFigli: %d\n", numFigli);
  sleep(2);
  for(int i = 0; i < numFigli; i++){
    son = fork();
    if(son == 0){
      //apro fifo in append, genero random e scrivo su fifo
      fd = open(fifoName, O_APPEND|O_CREAT);
      if(fd == -1){
        printf("Son %d, errore apertura fifo\n", getpid());
        exit(getpid());
      }
      rNum = (rand()%10);
      char buff[2];
      buff[0] = rNum+'0';
      buff[1] = '\0';
      printf("Generato %s, pid son %d\n", buff, getpid());
      write(fd, buff, 2);
      close(fd);
      exit(1);
    }
  }
  while(wait(NULL)>0);
  //procedo a leggere la fifo e sommare i valori
  fd = open(fifoName, O_RDONLY);
  int bytesRead = -1;
  char buffer[50];
  int letto = 0, sum = 0;
  do{
    bytesRead = read(fd, buffer, 3);
    letto = atoi(buffer);
    printf("Ho letto %s, pid %d\n", buffer, getpid());
    sum += letto;
  }while(bytesRead > 0);
  close(fd);
  printf("Somma dei numeri: %d\n", sum);
  return 0;
}
