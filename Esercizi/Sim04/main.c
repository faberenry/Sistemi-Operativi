#include <stdio.h>
#include <unistd.h> 
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>

const char *fileLog;

void scritturaFile(char bufferLog[])
{
  FILE *output = NULL;
  output = fopen(fileLog, "a+");
  if(output != NULL)
  {
    fprintf(output, bufferLog, strlen(bufferLog)+1);
  }
  fclose(output);
}

void filgioScriveLog(int pidProcessA, int signo);

//handler segnali
void handler(int signo, siginfo_t *info, void *empty)
{
  printf("Segnale ricevuto: %d\n", signo);
  int pidProcessA = info->si_value.sival_int;
  //ricevo segnale da processo A e rimando segnale a processo A
  if(signo == SIGUSR1){ 
    printf("Ricevuto %d, rispondo a %d\n", signo, info->si_value.sival_int); 
    union sigval value;
    value.sival_int = getpid(); //rispondo mandando mio pid per confermare che funzioni
    filgioScriveLog(pidProcessA, signo);
    sigqueue(pidProcessA, SIGUSR1, value);
  }

  //ricevo segnale da processo A e mando segnale a un processo B che rimanda processo A segnale
  if(signo == SIGUSR2){
    //genero figlio che simula il processo B per mandare segnale al processo A
    int son = fork();
    if(son == 0)
    {
      filgioScriveLog(pidProcessA, signo);
      union sigval value;
      value.sival_int = getpid(); //rispondo mandando mio pid per confermare che sia un altro processo
      sigqueue(pidProcessA, SIGUSR2, value);
    }
  }

  //ricevo CTRL-C scrivo su file log STOP\n
  if(signo == SIGINT){
    filgioScriveLog(pidProcessA, signo);
    kill(getpid(), SIGTERM);
  }

}

void handlerTerm(int signo){
  if(signo == SIGTERM){
    kill(getpid(), SIGTERM);
  }
}

int main(int argc, char const *argv[])
{
  if(argc != 2){
    char *buff = "Usage: ./SignalProxy <pathToLogFile>\n";
    write(STDERR_FILENO, buff, strlen(buff));
    return 1;
  }
  fileLog = argv[1];
  printf("Pid SP: %d\n", getpid());
  
  //associazione segnali all'handler
  //siccome mi serve passare anche il pid del processo A gli associo tramite sigaction (in teoria)
  struct sigaction sa;
  sa.sa_sigaction = handler;
  sa.sa_flags = SA_SIGINFO;
  sigaction(SIGUSR1, &sa, 0);
  sigaction(SIGUSR2, &sa, 0);
  sigaction(SIGINT, &sa, 0);

  while(1); //mi attivo e aspetto segnale da parte di qualcuno
  return 0;
}

void filgioScriveLog(int pidProcessA, int signo)
{
   //genero un altro figlio per scrivere sul file di log quello appena avvenuto
  int loggerSon = fork();
  if(loggerSon == 0)
  {
    sleep(3); //aspetto 3 secondi
    struct sigaction sa;
    sa.sa_sigaction = NULL;
    sigaction(SIGINT, &sa, 0);
    signal(SIGTERM, handlerTerm);
    //faccio l'azione in base al segnale ricevuto
    char bufferLog[31]; //assumo 30 di dimensione massima per scrittura su file di log
    if(signo == SIGUSR1 || signo == SIGUSR2)
    {
      sprintf(bufferLog, "%d-%d\n", pidProcessA, signo);
      printf("Scrivo sul file di log %s\n", bufferLog);
    }else if(signo == SIGINT){
      strcpy(bufferLog, "stop\n");
      printf("Ricevuto CTRL-C, scrivo %s", bufferLog);
    }
    //scrivo su fileLog
    scritturaFile(bufferLog);
  }
}