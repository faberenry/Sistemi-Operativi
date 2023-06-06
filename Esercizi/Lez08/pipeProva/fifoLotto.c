/*N figli generano numero casuale e scrivono su fifo <num,pid>, padre genera numero casuale e 
dice che figlio ha vinto, se ne eiste uno*/
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

struct coppia
{
  int numeroPensato;
  int pidSon;
};

void clearFifo(char *fifoName)
{
  int fd = open(fifoName, O_WRONLY|O_TRUNC, 0755);
  close(fd);
}

int main(int argc, char const *argv[])
{
  int fileDescriptor;
  char *fifoName = "/tmp/fifoLotto";
  //clearFifo(fifoName);
  //clearFileProva
  int fd = open("prova.txt", O_WRONLY|O_TRUNC, 0755);
  close(fd);
  char buffer[50];
  if(argc != 2){
    printf("Usage: ./fifoLotto.o <nGiocatori>\n");
    exit(1);
  }else{
    int numFigli = atoi(argv[1]);
    printf("Genero %d figli\n", numFigli);
    mkfifo(fifoName, S_IRUSR|S_IWUSR);
    perror("Fifo creata?");
    for (int i = 0; i < numFigli; i++)
    {
      int son = fork();
      if(son == 0){
        srand(time(NULL));
        int randNumber = (rand()%10);
        printf("Son %d, rand %d\n", getpid(), randNumber);
        char str[20];
        sprintf(str, "%d,%d\n", getpid(), randNumber);
        /*fileDescriptor = open(fifoName, O_WRONLY|O_CREAT|O_APPEND);
        write(fileDescriptor, str, strlen(str));
        close(fileDescriptor);*/
        int fd = open("prova.txt", O_WRONLY | O_APPEND | O_CREAT, 0755);
        write(fd, str, strlen(str));
        close(fd);
        exit(1);
      }
    }
    while(wait(NULL)>0);
    // printf("Ho aspettato tutti i figli\n");
    fileDescriptor = open("prova.txt", O_RDONLY, 0755);
    int bytesToRead = -1;
    do{
      bytesToRead = read(fileDescriptor, buffer, sizeof(buffer));
      printf("Letto: %s", buffer);
    }while(bytesToRead > 0);
    close(fileDescriptor);
  }
  return 0;
}

