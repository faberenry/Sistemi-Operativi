#include <stdio.h>
#include <unistd.h> 
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/select.h>

#define DIM 51

const char *clientStr = "client";
const char *serverStr = "server";
const char *fileLogName;
int counterSons = 0; //counter per tenere conto di quanti figli sono ancora attivi
int sons[10];

//SIGINT -> scrivo figli presenti, counterSons, ed esco
  //SIGUSR1 -> genero figlio, scrivo suo pid su file di log "+getpid()\n", "stdout: [server] +pid", aumento counterSons
  //SIGUSR2 -> termino figlio a caso, scrivo pid su file di log "-getpid()\n" scrivo, stdout: [server] -pid, diminuisco counterSons, 
                // se counterSons == 0 allora scrivo lui, anche perchè no problema di mutua esclusività perchè gestito solo da Server
void serverHandler(int signo)
{
  printf("Ricevuto segnale %d\n", signo);
  if(signo == SIGINT){
    
  }

  if(signo == SIGUSR1){
    printf("Ricevuto segnale SIGUSR1, genero un figlio\n");
    sons[counterSons] = fork();
    if(sons[counterSons] != 0)
    {
      counterSons++;
    }

    if(sons[counterSons] == 0)
    {
      
    }
  }
}

// static void clientHandler(int signo)
static void clientHandler(int signo, siginfo_t *info, void *empty)
{
  printf("Ricevuto segnale %d\n", signo);
  if(signo == SIGINT)
  {
    printf("Client terminato\n");
    kill(-getpid(), SIGTERM);
    return;
  }

  if(signo == SIGUSR1)
  {
    char buf[2];
    const char *pidServerStr = info->si_ptr;
    printf("Inviato segnale SIGUSR1, comunico a %s di creare un figlio\n", pidServerStr);
    int pidServer = atoi(pidServerStr);
    sigqueue(pidServer, SIGUSR1, (union sigval){ .sival_ptr = (void *)buf});
  }

  if(signo == SIGUSR2)
  {
    printf("Inviato segnale SIGUSR2\n");
  }

}

int main(int argc, char const *argv[])
{

  if(argc != 3){ //numero di argomenti sbagliati 
    fprintf(stderr, "Usage: ./app <server/client> <filepath>\n");
    return 1;
  }
  fileLogName = argv[2];

  char tipologia[DIM];
  char buffer[DIM];
  strcpy(tipologia, argv[1]);
  if(strcmp(tipologia, clientStr) == 0)
  {
    struct sigaction sa;
    sa.sa_sigaction = &clientHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags |= SA_SIGINFO; // Use sa sigaction
    sigaction(SIGUSR1,&sa,NULL);
    sigaction(SIGUSR2,&sa,NULL);
    sigaction(SIGINT,&sa,NULL);

    int pidServer = 0;
    char c;
    fd_set readfds;
    struct timeval timeout;
    fprintf(stdout, "Loggato come %s\n", tipologia);
    //leggo prima riga e stampo il risultato
    FILE *input = NULL;
    do{
      input = fopen(argv[2], "r");
      if(input != NULL) fscanf(input, "%d", &pidServer);
    }while(input == NULL || pidServer == 0);
    fclose(input);
    printf("[client] server: %d\n", pidServer);
    char msg[20];
    sprintf(msg, "%d", pidServer);
    int n = 1;
    while(1)
    {
      scanf("%c", &c);
      if(c == '+'){
        sigqueue(getpid(), SIGUSR1, (union sigval){ .sival_ptr = (void *)msg });
      }else if(c == '-'){
        sigqueue(getpid(), SIGUSR2, (union sigval){ .sival_ptr = (void *)msg });
      }else if(c == '\n' && !n^1){
        sigqueue(getpid(), SIGUSR2, (union sigval){ .sival_ptr = (void *)msg });
      }
      fflush(stdin);
    }
  }
  
  if(strcmp(tipologia, serverStr) == 0)
  {
    signal(SIGUSR1, serverHandler);
    signal(SIGUSR2, serverHandler);
    signal(SIGINT, serverHandler);

    fprintf(stdout, "Loggato come %s\n", tipologia);
    //scrivo su file mio pid
    sprintf(buffer, "%d\n", getpid());
    int fd = open(argv[2], O_WRONLY|O_CREAT, 0755);
    if(fd == -1){
      fprintf(stderr, "Errore apertura file\n");
      return 2;
    }
    write(fd, buffer, strlen(buffer));
    close(fd);
    fprintf(stdout, "[server:%d]\n", getpid());
    while(1); //rimango attivo in attesa di segnali
  }
  return 0;
}


/*
server riceve SIGINT
  char buff[10];
  printf("Scrivo figli presenti %d\n", counterSons);
  int fd = open(fd, O_APPEND, 0755);
  sprintf(buff, "%d\n", counterSons);
  if(fd == -1){
    fprintf(stderr, "Errore apertura file\n");
    return 2;
  }
  write(fd, buff, strlen(buff));
  close(fd);
*/

/*lettura da stdin 
FD_ZERO(&readfds);
      FD_SET(STDIN_FILENO, &readfds);

      timeout.tv_sec = 0;
      timeout.tv_usec = 0;

      // Check if input is available on stdin
      int ready = select(STDIN_FILENO + 1, &readfds, NULL, NULL, NULL);

      if (ready == -1) {
        perror("select");
        exit(EXIT_FAILURE);
      } else if (ready == 1){
        read(STDIN_FILENO, &c, sizeof(char));
        //controllo valore di c
        if(c == '+'){
          kill(getpid(), SIGUSR1);
        }
        else if(c == 13){ //ho premuto tast invio
          kill(getpid(), SIGUSR2);
        }else{
          printf("Value of c: %c\n", c);
        }
      }
*/

/*
    // signal(SIGUSR1, clientHandler);
    //signal(SIGUSR2, serverHandler);
    // signal(SIGINT, clientHandler);
    /*struct sigaction sa;
    sa.sa_sigaction = &clientHandler;
    sa.sa_flags = getpid(); 
    sigaction(SIGUSR2, &sa, 0);*/