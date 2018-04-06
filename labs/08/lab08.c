#include <stdio.h>


int main(int argc, char **argv)
{
    FILE *fp;
    char c;

    if (argc != 2) {
	fprintf(stderr, "usage: %s file1\n", argv[0]);
	return 1;
    }
	
    if ((fp = fopen(argv[1], "r")) != NULL) {
    	while ((c = getc(fp)) != EOF)
		putchar(c);
    	fclose(fp);
    } else {
	perror(argv[1]);
	return 1;
    }
    return 0;
}
