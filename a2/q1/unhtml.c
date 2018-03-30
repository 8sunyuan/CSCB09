#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
	int status = 0;
	FILE *fp;
	extern void process(FILE *fp);
	if (argc == 1) {
		process(stdin);
	} else {
		for (argc--, argv++; argc > 0 ; argc--, argv++) {
			if (strcmp(*argv, "-") == 0) {
				process(stdin);
	    	} else if ((fp = fopen(*argv, "r")) == NULL) {
				perror(*argv);
				fprintf(stderr, "usage: %s [file ...]\n", argv[0]);
				return(1);
	    	} else {
				process(fp);
				fclose(fp);
	    	}
		}
	}
	return(status);
}

void process(FILE *fp) {
    int c;
	int count = 0;
    while ((c = getc(fp)) != EOF) {
		if (c == '<')
			count++;
		else if (c == '>')
			count--;
		else if (!count) // If count is 0, character is outside html tag 
			putchar(c);
	}
}