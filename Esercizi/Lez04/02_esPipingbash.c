/*Input ls /tmp | ./inv.out */
#include <stdio.h>

int main(int argc, char const *argv[]) {
  int letto, scritto;
  const int offset = 32;
  while ((letto = getchar()) != EOF){ 
    scritto = letto;
    if( letto >= 'A' && letto <= 'Z') scritto += offset;
    if( letto >= 'a' && letto <= 'z') scritto -= offset;
    putchar(scritto);
  }
  return 0;
}
/*
Comandi come getchar(), putchar() interagiscono con gli stdin e stdout della bash se viene passato un 
comando tramite esssa, ove getchar() legge da STDIN mentre putchar() mette a schermo nella bash il valore utilizzando STDOUT
*/
