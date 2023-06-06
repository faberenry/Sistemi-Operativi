#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
  int fileDescriptor;
  char *fifoName = "/tmp/fifofork1";
  char buffer[50];
  mkfifo(fifoName, S_IRUSR|S_IWUSR);
  perror("Fifo creata?");
  int son = fork();
  if(son == 0){
    //figlio leggo quello che scrive padre
    sleep(2);
    fileDescriptor = open(fifoName, O_RDONLY);
    read(fileDescriptor, buffer, sizeof(buffer));
    close(fileDescriptor);
    printf("Son has read: %s\n", buffer);
    fileDescriptor = open(fifoName, O_WRONLY);
    write(fileDescriptor, "Ciao padre", strlen("Sono figlio")+1);
    close(fileDescriptor);
    exit(1);
  }else{
    //padre: scrivo a figlio
    fileDescriptor = open(fifoName, O_WRONLY|O_CREAT);
    write(fileDescriptor, "Ciao Figlio", strlen("Ciao Figlio")+1);
    close(fileDescriptor);
    fileDescriptor = open(fifoName, O_RDONLY);
    read(fileDescriptor, buffer, sizeof(buffer));
    close(fileDescriptor);
    printf("Father has read: %s\n", buffer);
  }
  return 0;
}

