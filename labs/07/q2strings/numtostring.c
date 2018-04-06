#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	int num;
	int status = 0;
	char *str;
	str = malloc(3 * sizeof(char));

	if (argc == 2) {
		num = atoi(argv[1]);
		if (num > 99 || num < 10) status = 1;
	} else {
		status = 1;
	}

	if (!status){
		str[0] = num / 10 + '0';
		str[1] = num % 10 + '0';
		str[2] = '\0';
		printf("%s\n", str);
	}
	return status;
}