#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

void usr1_handler(int signo, siginfo_t *info, void *context) {
  const char *msg = info->si_ptr;
  char buf[100];
  sprintf(buf, "Signo: %d\n", signo);
  write(STDOUT_FILENO, buf, strlen(buf));
  if(signo == SIGUSR1){
    write(STDOUT_FILENO, msg, strlen(msg));
  }else if(signo == SIGUSR2){
    write(STDOUT_FILENO, "USR2\n", strlen("USR2\n"));
  }
}


int main(void) {
  struct sigaction sa;
  sa.sa_sigaction = usr1_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_SIGINFO;
  sigaction(SIGUSR1, &sa, NULL);
  sigaction(SIGUSR2, &sa, NULL);

  const char *msg = "SIGUSR1 was called.\n";
  const char *msg2 = "SIGUSR2 was called.\n";
  sigqueue(getpid(), SIGUSR2, (union sigval){ .sival_ptr = (void *)msg2 });
  sigqueue(getpid(), SIGUSR1, (union sigval){ .sival_ptr = (void *)msg });

  return 0;
}