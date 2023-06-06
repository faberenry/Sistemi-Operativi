#include <stdio.h>
#include <unistd.h> 
#include <string.h>
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>

#define MAXSONS 10

int sonsCounter; //utlizzato anche per tenere conto di quanti processi sono attivi
int sonArray[MAXSONS] = {1, 1, 1, 1, 1, 1, 1, 1, 1};

int valid()
{
  return (sonsCounter <= 10 && sonsCounter >= 1);
}

void scriviPid(const char *target, int pidProcess)
{
  char buffer[20]; //creazione buffer per scrittura pid 
  sprintf(buffer, "%d\n", pidProcess);
  int fileDescriptor = open(target, O_WRONLY|O_APPEND, 0755);
  write(fileDescriptor, buffer, strlen(buffer));
  close(fileDescriptor);
}

void managerHandler(int signo, siginfo_t *info, void *empty)
{
  if(signo == SIGUSR1)
  {
    if(sonsCounter == 0) //uccido il managaer
    {
      kill(getpid(), SIGTERM); //posso usare getpid() perch\ace è il manager che fa handling dei processi
    }else if(sonsCounter > 0)
    {
      int pidProcessoX = info->si_value.sival_int;
      union sigval value;
      value.sival_int = info->si_value.sival_int;
      sigqueue(pidProcessoX, SIGUSR1, value);
      
      kill(sonArray[sonsCounter-1], SIGTERM);
      sonsCounter--;
      printf("Ho ricevuto un signal USR1, uccido un figlio. Rimanenti: %d\n", sonsCounter);
    }
  }
  else if(signo == SIGTERM){
    //uccido tutti i presenti
    printf("Manger: ricevuto SIGTERM, uccido tutti i figli\n");
    for(int i = 0; i < sonsCounter; i++)
    {
      kill(sonArray[i], SIGTERM);
    }
    kill(getpid(), SIGTERM);
  }
  else{
    printf("signo ricevuto: %d\n", signo);
  }
}

void sonHandler(int signo, siginfo_t *info, void *empty)
{
  if(signo == SIGUSR1)
  {
    char buf[20];
    sprintf(buf, "%d", getpid()); 
    sigqueue(info->si_value.sival_int, SIGUSR2, (union sigval){ .sival_ptr = (void *)buf}); //invio pid del processo figlio per conferma in output che sia il figlio a rispondere
  }
}

int main(int argc, char const *argv[])
{
  if(argc != 3){
    write(STDERR_FILENO, "Usage: ./program <target> <number>\n", strlen("Usage: ./program <target> <number>\n"));
    return 3;
  }

  sonsCounter = atoi(argv[2]);
  if(!valid()){
    write(STDERR_FILENO, "Valore <number> non valido\n", strlen("Valore <number> non valido\n"));
    return 4;
  }

  const char *target = argv[1];
  int fileDescriptor = open(target, O_WRONLY|O_CREAT|O_EXCL, 0777);
  if(fileDescriptor == -1){
    write(STDERR_FILENO, "Errore apertura file\n", strlen("Errore apertura file\n"));
    return 5;
  }
  close(fileDescriptor);
  printf("Padre: %d\n", getpid());
  scriviPid(target, getpid()); //scrivo pid primo processo

  int manager = fork();
  if(manager == 0)
  {
    printf("Manager: %d\n", getpid());
    scriviPid(target, getpid());
    struct sigaction saManager;
    sigset_t modManager;
    sigaddset(&modManager, SIGALRM);
    saManager.sa_sigaction = managerHandler;
    saManager.sa_mask = modManager;
    sigaction(SIGUSR1, &saManager, 0);
    sigaction(SIGTERM, &saManager, 0);
    int status = 0;
    for(int i = 0; i < sonsCounter; i++)
    {
      if(sonArray[i] != 0) //controllo per prevenzione di figli che generano altri figli, solo padre deve generare i figli
      {
        sonArray[i] = fork();
        if(sonArray[i] == 0)
        {
          sigset_t mod;
          struct sigaction sa;
          sa.sa_sigaction = sonHandler;
          sigaddset(&mod, SIGCHLD);
          sigaddset(&mod, SIGCONT);
          sigemptyset(&sa.sa_mask);
          sa.sa_mask = mod;
          sigaction(SIGUSR1, &sa, 0);
          printf("Nipote %d: %d\n", i, getpid());
          scriviPid(target, getpid());
          while(1);
        }
      }
    }
    while(1);
  }
  while(1);
  return 0;
}
