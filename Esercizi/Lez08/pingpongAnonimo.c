#include <stdio.h> 
#include <unistd.h> 
#include <string.h>
#include <sys/wait.h> 
#define READ 0 
#define WRITE 1

int main(int argc, char const *argv[])
{
  int pipePing[2], pipePong[2];
  char *stop = "fine";
  char buffer[50];
  pipe(pipePing);
  pipe(pipePong);
  int pong = fork();
  if(pong == 0){
    //leggo dalla pipe con pong
    while(1)
    {
      close(pipePing[WRITE]);
      close(pipePong[READ]);
      int r = read(pipePing[READ], &buffer, 50);
      close(pipePing[READ]);
      printf("Sono pong, ho letto: %s\n", buffer);
      //scrivo pong nella pipe
      write(pipePong[WRITE], "PONG\n", 6);
      close(pipePong[WRITE]);
      // sleep(4);
    }
  }else{
    //scrivo sulla pipe con PING
    while(1)
    {
      close(pipePing[READ]);
      close(pipePong[WRITE]);
      write(pipePing[WRITE], "PING\n", 6);
      close(pipePing[WRITE]);
      int r = read(pipePong[READ], &buffer, 50);
      close(pipePong[READ]);
      printf("Sono ping, ho letto: %s\n", buffer);
      sleep(4);
    }
  }
  while(wait(NULL)>0);
  return 0;
}
