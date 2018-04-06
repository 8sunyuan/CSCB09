#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char **argv) {
  if (argc != 2)
    return 0;
  int result, respawn = 0;

  FILE *fp;
  char *line, *p;
  line = malloc(100*sizeof(char));

  if ((fp = fopen(argv[1], "r")) == NULL) {
    perror(argv[1]);
    return 1;
  }

  while((fgets(line, 100, fp)) != NULL) {
    p = line;
    if ((p = strchr(line, '\n')))
      *p = '\0';


    do {
      result = fork();
      if (result == -1) {
        perror("fork:");
        return(1);
      } else if (result == 0) { // Child process
        execl("/bin/sh", "sh", "-c", line, (char *)NULL);
        perror("/bin/sh");
        return (127);
      } else if (result > 0) {	// Parent Process
        if (waitpid(result, NULL, 0) < 0)
          perror("wait");
      }
    } while (respawn);

  }
}
