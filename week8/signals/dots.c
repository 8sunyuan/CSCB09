#include <stdio.h>
#include <unistd.h>

int main() {
	int c = 0;
	while (c++ < 999999999) {
		fprintf(stderr, ".");
		sleep(1);
	}
	return 0;

}
