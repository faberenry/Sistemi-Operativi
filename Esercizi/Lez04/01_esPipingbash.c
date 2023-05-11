/*
 Input: echo “hi how are you” | ./pip.out
*/
#define MAXBUF 10 
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
  char buf[MAXBUF];
  fgets(buf, sizeof(buf), stdin);
  printf("%s\n", buf);
  return 0;
}

