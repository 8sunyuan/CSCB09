#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    char buf[1000], filename[1000], *p;
    int status;
    while (printf("$ "), fgets(buf, sizeof buf, stdin)) {
	     printf("File to output to: ");
	     if (fgets(filename, sizeof filename, stdin) == NULL)
	      return(0);
	     if ((p = strchr(filename, '\n')))
	      *p = '\0';
	     switch (fork()) {
	        case -1:
	         perror("fork");
	         return(1);
	        case 0:
	         /* child */
	         close(1);
	         if (open(filename, O_WRONLY|O_CREAT|O_TRUNC, 0666) < 0) {
		           perror(filename);
		           return(127);
	         }
    	     execl("/bin/sh", "sh", "-c", buf, (char *)NULL);
    	     perror("/bin/sh");
    	     return(127);
	        default:
	         if (wait(&status) < 0)
		         perror("wait");
	         else
		         printf("exit status %d\n", WEXITSTATUS(status));
       }
    }
    return(0);
}
