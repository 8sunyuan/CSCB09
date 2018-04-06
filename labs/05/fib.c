#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	int n;
	if (argc == 2 && (n = atoi(argv[1])) >= 0 && n <= 46) {
		int fib[47];
		for (int i = 0; i <= n; i++) {
			if (i <= 1) {
				fib[i] = i;
			}
			else {
				fib[i] = fib[i-1] + fib[i-2];
			}
		}
		printf("%d\n", fib[n]);
		return 0;
	}
 	fprintf(stderr, "usage: %s file1 \n", argv[0]);	 
	return 1;
}
