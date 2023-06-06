#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <unistd.h> 
#include <string.h>
#include <sys/wait.h> 
#define READ 0 
#define WRITE 1

int main(int argc, char const *argv[])
{
  srand(time(NULL));
  int pipeFather[2], pipeSon[2];
  int numMex;
  char *stop = "fine";
  char buffer[50];
  pipe(pipeFather);
  pipe(pipeSon);
  int pong = fork();
  if(argc == 2) numMex = atoi(argv[1]);
  else numMex = (rand()%10);
  if(pong == 0){
    //leggo dalla pipe con pong
    // close(pipeFather[WRITE]);
    // close(pipeSon[READ]);
    while(1)
    {
      read(pipeSon[READ], &buffer, sizeof(buffer));
      printf("S-%s\n", buffer);
      if(strcmp(buffer, "fine") == 0) 
        break;
      write(pipeFather[WRITE], "ping", strlen("ping"));
      sleep(1);
    }
  }else{
    //scrivo sulla pipe con PING
    // close(pipeSon[WRITE]);
    // close(pipeFather[READ]);
    while(1)
    {
      if(numMex > 0) write(pipeSon[WRITE], "pong", strlen("pong"));
      else {
        write(pipeSon[WRITE], "fine", strlen("fine"));
        break;
      }
      read(pipeFather[READ], &buffer, sizeof(buffer));
      printf("F-%s\n", buffer);
      numMex--;
      sleep(1);
    }
  }
  while(wait(NULL)>0);
  return 0;
}
