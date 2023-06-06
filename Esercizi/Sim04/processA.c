#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void handler(int signo, siginfo_t *info, void *empty)
{
  printf("Ricevuto segnale %d da processo %d\n", signo, info->si_value.sival_int);
}

int main(int argc, char const *argv[])
{
  union sigval value;
  value.sival_int = getpid();
  int scelta = atoi(argv[2]);
  if(scelta == 1) sigqueue(atoi(argv[1]),SIGUSR1,value);
  else if(scelta == 2) sigqueue(atoi(argv[1]), SIGUSR2, value);

  struct sigaction sa;
  sa.sa_sigaction = handler;
  sa.sa_flags = SA_SIGINFO;
  sigaction(SIGUSR1, &sa, 0);
  sigaction(SIGUSR2, &sa, 0);
  
  while(1);

  return 0;
}
