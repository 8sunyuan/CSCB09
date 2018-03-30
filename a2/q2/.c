#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	int opt;
	int width = 80;
	int status, punc = 0;
	//extern char* optarg;
	FILE *fp;
	extern void process(FILE *fp, int width, int punc);
	while ((opt = getopt(argc, argv, "l:p")) != EOF){
		switch (opt) {
			case 'l':
				width = atoi(optarg);
				break;
			case 'p':
				punc = 1;			
				break;
			default:
				abort();
		}
	}

	if (optind == argc) {
		process(stdin, width, punc);
	} else if (optind == argc - 1) {
		if ((fp = fopen(argv[optind], "r")) == NULL) {
			perror(argv[optind]);
			status = 1;
		} else {
			process(fp, width, punc);
		}
	}
	return status;
}

void process(FILE *fp, int width, int punc) {
	int linebreak = -1;
	int i = 0;
	char c;
	char *line, *leftover;
	line = malloc((width+1) * sizeof(char));
	leftover = malloc((width+1) * sizeof(char));
	while((c = getc(fp)) != EOF) {
		if (i < width) { // Within length of line
			line[i] = c;
			if (c == ' ') linebreak = i;
			else if (punc && c == ',') linebreak = i;
		} else { // Reached max length of line
			if (linebreak >= 0){
				/* loop from linebreak to end of line and save that as leftover string
				   print line
				   strcpy leftover to newline
				*/
				strncpy(leftover, line + linebreak, width);
				printf("%s\n", line);
				strcpy(line, leftover);
				strcpy(leftover, "");
				i = strlen(line);
			} else { // Nowhere to break, print entire line
				i = 0;
			}
			linebreak = -1;
		}
	}
}



