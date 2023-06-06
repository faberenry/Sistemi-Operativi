/*
Creare un’applicazione in C che gestisca un albero di processi tramite dei segnali. In particolare, il
programma, una volta lanciato, deve accettare i seguenti comandi:
  ● c(n) --> crea nuovi figli al livello n
  ● k n --> termina i figli al livello n
  ● p --> stampa in output l’albero dei processi
  ● q --> termina il processo dopo aver terminato tutti i figli
L’intera comunicazione deve avvenire esclusivamente tramite segnali inviati dal processo principale.
L’output del comando ‘p’ non deve essere ordinato ma deve essere ben chiaro il livello di ogni processo
(per esempio usando la tabulazione).

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <signal.h>



void handlerCrea(int signNo){

}

int main()
{
  //imposto i signal
  signal(SIGUSR1, handlerCrea);
  signal(SIGUSR2, handlerKiller);

  char scelta[2]={'a','a'};
  while(scelta[0]!='q')
  {
    printf("Next command: ");
    //scanf(scelta,2);
    scanf("%s", scelta);
    // printf("Scelta effettuata: %c\n", scelta[0]);
    sleep(3);
    switch (scelta[0])
    {
      case 'c':
        /* code */
        //crea figlio a livello (n) dell'albero
        break;
      case 'p':
        //stampa albero
        break;
      case 'q':
        //termina processo principale dopo aver terminato i figli
        return 0;
        break;
      case 'k':
        //termina i figli a livello (n)
        break;
      default:
        printf("Scelte poss: c(n),p,q,k(n)\n");
        break;
    }
  }
  return 0;
}
