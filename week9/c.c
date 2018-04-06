#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {
  extern char **environ;
  if (argc > 1 && strcmp(argv[1], "a.out") == 0)
    fprintf(stderr, "I wont let you ls that!\n");
  else
    execve("/bin/ls", environ);
  return 1;

}
