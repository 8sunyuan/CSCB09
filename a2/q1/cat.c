#include <stdio.h>
#include <string.h>


int main(int argc, char **argv)
{
    int i;
    FILE *fp;
    extern void process(FILE *fp);

    if (argc == 1) {
	process(stdin);
    } else {
	for (i = 1; i < argc; i++) {
	    if (strcmp(argv[i], "-") == 0) {
		process(stdin);
	    } else if ((fp = fopen(argv[i], "r")) == NULL) {
		perror(argv[i]);
		return(1);
	    } else {
		process(fp);
		fclose(fp);
	    }
	}
    }
    return(0);
}


void process(FILE *fp)
{
    int c;
    while ((c = getc(fp)) != EOF)
	putchar(c);
}