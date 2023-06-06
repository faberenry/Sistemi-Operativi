#include <stdio.h>

int main(int argc, char **argv[])
{
  if(argc == 1)
  {
    printf("Sono binary %s\n", argv[0]);
  }else{
    printf("Sono un processo\n");
  }
  return 0;
}
