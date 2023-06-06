#include <stdio.h>
#include <unistd.h> 
#include <string.h>
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <fcntl.h>
#include <stdlib.h>


int main(int argc, char const *argv[])
{
  /* code */
  if(argc != 3){
    fprintf(stderr, "Usage: ./program <target> <num>\n");
    return 3;
  }
  int numIn = atoi(argv[2]);
  if(numIn <= 0 || numIn > 10){
    fprintf(stderr, "Num compreso tra 1 e 10\n");
    return 4;
  }
  int fd = open(argv[1], O_WRONLY|O_CREAT, 0755);
  printf("FileDescriptor= %d\n", fd);
  if(fd == -1){
    fprintf(stderr, "Errore apertura file %s\n", argv[1]);
    return 5;
  }
  char buffer[50];
  sprintf(buffer, "%d\n", getpid());
  write(fd, buffer, strlen(buffer) ); //scrivo pid primo processo
  close(fd);
  int manager = fork();
  if(manager == 0){
    sprintf(buffer, "%d\n", getpid());
    fd = open(argv[1], O_APPEND, 0755);
    printf("manager %d fd: %d\n", getpid(), fd);
    if(fd != -1) write(fd, buffer, strlen(buffer));
    close(fd);
    int nipoti[numIn];
    for(int i = 0; i < numIn; i++){
      nipoti[i] = fork();
      if(nipoti[i] == 0){
        sprintf(buffer, "%d\n", getpid());
        fd = open(argv[1], O_APPEND, 0775);
        printf("nipote %d fd: %d\n", getpid(), fd);
        if(fd != -1) write(fd, buffer, strlen(buffer) );
        close(fd);
        printf("nipote %d rimane attivo\n", getpid());
        while(1); //nipoti rimangono attivi
      }
    }
    if(nipoti != 0){
      printf("Manager %d rimane attivo\n", getpid());
      while(1); //manager rimane attivo
    }
  }
  else{
    sleep(1);
    printf("Processo principale %d termina\n", getpid());
  }
  return 0;
}
