#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

int sum = 0;

void myHandler(int signo, siginfo_t *info, void *empty){
  int recived = info->si_value.sival_int;
  printf("Ricevuto %d\n", recived);
  sum += recived;
  printf("Somma: %d\n", sum);
}

void handler(int signal)
{
  printf("Segnale ricevuto %d\n", signal);
}

int main(){
  srand(time(NULL));
  signal(SIGUSR1, handler);
  int son = fork();
  if(son == 0){
    struct sigaction sa;
    sa.sa_sigaction = myHandler;
    sigemptyset(&sa.sa_mask); 
    sa.sa_flags = SA_SIGINFO; // Use sa_sigaction
    sigaction(SIGUSR1,&sa,NULL);
  }else{
    union sigval value;
    int randomNumber;
    while(sum < 10){
      randomNumber = (rand()%10);
      printf("Generato: %d\n", randomNumber);
      value.sival_int = randomNumber; 
      sigqueue(son,SIGUSR1,value);
      sleep(3);
    }
   sigqueue(son,SIGSTOP,value);
  }
  return 0;
}

/*
padre, 
  while(1)
    genero random
    manda con segnale random
  
figlio 
  while(1)
  somma valori
*/
