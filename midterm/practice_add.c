#include <stdio.h>

int main() {
	int x;
	int sum;
	int status = 0;
	while ((status == scanf("%d", &x)) == 1) {
		sum += x;
	}
	if (status != EOF) {
		fprintf(stderr, "add: non-numeric input");
		return 1;
	}
	printf("%d\n", sum);
	return 0;
}