#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

int main() {
	int i,j,y, num;
	for (i = 0; i < 243; i++) {
		putchar('1');
		num = i;
		for (j = 2; j <= 6; j++) {
			y = num % 3;
			if (y == 0) {
				putchar('0' + j);
			} 
			else if (y == 1) {
				putchar('+');
				putchar('0' + j);
			}
			else {
				putchar(('-'));
				putchar('0' + j);
			}
			num = num/3;
		}
		printf("\n");
	}
}
