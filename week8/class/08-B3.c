#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    char buf[1000];
    int status;
    while (printf("$ "), fgets(buf, sizeof buf, stdin)) {
    	switch (fork()) {
    	case -1:
    	    perror("fork");
    	    return(1);
    	case 0:
    	    /* child */
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
