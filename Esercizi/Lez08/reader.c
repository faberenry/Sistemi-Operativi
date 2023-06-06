#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
  int fd; 
  char *fifoName = "/fifo2";
  mkfifo(fifoName, S_IRUSR|S_IWUSR);
  perror("Created?");
  char str1[80];
  char *str2 = "I'm reader";
  fd = open(fifoName, O_RDONLY);
  read(fd, str1, 80);
  close(fd);
  printf("Writer is writing: %s\n", str1);
  fd = open(fifoName, O_WRONLY);
  write(fd, str2, strlen(str2)+1);
  close(fd);
  return 0;
}
