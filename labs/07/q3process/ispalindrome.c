#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main (int argc, char **argv) {
	int status = 0;
	char *p, *q;
	if (argc == 2) {
		p = *(argv+1);
		q = *(argv+1);
		while (*q != '\0')
			q++;
		q--;
		while (p < q) {
			if (isalnum(*p) == 0) {
				p++;
				continue;
			} else if (isalnum(*q) == 0) {
				q--;
				continue;
			}
			if (tolower(*p) != tolower(*q)){ 							status = 1;
				break;
			}
			p++;
			q--;
		}
	} else {
		fprintf(stderr, "usage: %s file \n", argv[0]);
		status = 1;
	}
	return status;
}
