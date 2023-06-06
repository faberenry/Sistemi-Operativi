#include <stdio.h>
#include <unistd.h> 
#include <string.h>
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <fcntl.h>
#include <stdlib.h>

#define DIM 33

/*
new - crea coda definita da (nome,1), se esiste già stampa il suo id
put - +valAgg, mette il mesaggio scritto in valAgg all'interno della coda, se non esiste nessuna coda la crea
get - stampa il primo valore della queue (NULL se vuota), se non esiste la crea
del - elimina se esiste la coda
emp - stampa tutti i valori, la crea se non esiste
mov - sposta contenuto queue NOME in VALAGG, se NOME non esiste ERRORE, crea VALAGG se non esiste
      elimina la prima stampando gli spostamenti

OGNI AZIONE invia un segnale al processo PID, SIGUSR1 successo, SIGUSR2 insucesso

/tmp/prova - coda in cui lavoriamo
/tmp/provacopia - coda dove sposto le cose


./coda /tmp/prova put ciao 100
./coda /tmp/prova put ciao1 100
./coda /tmp/prova put ciao2 100
./coda /tmp/prova emp 100
./coda /tmp/prova get 100
./coda /tmp/prova mov /tmp/provacopia 100
./coda /tmp/provacopia del 100
*/

const int nAction = 6;
const char *actions[6] = {"new", "put", "get", "del", "emp", "mov"};
const int tipoMess = 20;

struct msg_buffer{
  long mtype;
  char mtext[DIM];
} sender, reciver;


int checkAction(char azione[]){
  for(int i = 0; i < nAction; i++){
    if(strcmp(azione, actions[i])==0){
      return i;
    }
  }
  return -1;
}

int createQueue(char *nome){
  creat(nome, 0777);
  key_t keyCoda = ftok(nome, 1);
  int idCoda = msgget(keyCoda, 0777 | IPC_CREAT | IPC_EXCL);
  if(idCoda == -1){
    idCoda = msgget(keyCoda, 0777);
    printf("ID coda: %d\n", idCoda);
  }
  return idCoda;
}

int removeQueue(int idCoda){
  if(idCoda != -1){
    printf("Elimino coda\n");
    int res = msgctl(idCoda,IPC_RMID,NULL);
    printf("Result of action: %d\n", res);
    return -1;
  }
  return 0;
}

int main(int argc, char const *argv[])
{
  key_t keyCoda;
  int coda = -1;
  int esito;
  char nome[DIM], azione[DIM], valoreAggiuntivo[DIM], pidBuff[DIM];
  valoreAggiuntivo[0] = '\0';
  if(argc == 4 || argc == 5){
    strcpy(nome, argv[1]);
    strcpy(azione, argv[2]);
    if(argc == 5){
      strcpy(valoreAggiuntivo, argv[3]);
      strcpy(pidBuff, argv[4]);
    }else{
      strcpy(pidBuff, argv[3]);
    }
    printf("Valori passati: \n nome \t: %s\n azione\t: %s \n valAgg\t: %s \n pid\t: %s \n", nome, azione, valoreAggiuntivo, pidBuff);
    int codAzione = checkAction(azione);
    switch (codAzione)
    {
    case 0: //new
      coda = createQueue(nome);
      
      break;
    case 1: //put
      if(strcmp(valoreAggiuntivo, "") == 0){
        printf("Errore\n");
      }else{
        printf("Aggiungo: %s\n", valoreAggiuntivo);
        if(coda == -1){
          coda = createQueue(nome);
          sender.mtype = tipoMess;
          strcpy(sender.mtext,valoreAggiuntivo);
          esito = msgsnd(coda , &sender, sizeof(sender.mtext),0);
        }
      }
      break;
    case 2: //get
      if(coda == -1){
        printf("GET\n");
        coda = createQueue(nome);
        esito = msgrcv(coda , &reciver, sizeof(reciver.mtext), tipoMess, IPC_NOWAIT);
        // printf("ID coda: %d\n", coda);
        // printf("Esito = %d\n", esito);
        printf("Received \"%s\"\n",reciver.mtext);
      }
      break;
    case 3: //del
      removeQueue(coda);
      break;
    case 4: //emp
      printf("EMP\n");
      if(coda == -1){
        coda = createQueue(nome);
      }
      while(1){
        esito = msgrcv(coda , &reciver, sizeof(reciver.mtext), tipoMess, 0);
        // printf("esito: %d\n",esito);
        if(esito == -1) break;
        else{
          printf("MsgRead: %s\n", reciver.mtext);
        }
        sleep(2);
      }
      break;
    case 5: //mov
      break;
    default:
      printf("Azione non riconsciuta\n");
      break;
    }
  }else{
    printf("Usage: ./coda <nome> <azione> <valAgg> <PID>\n");
  }
  return 0;
}
