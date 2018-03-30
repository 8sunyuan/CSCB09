#include <stdio.h>
#include <stdlib.h> 

int main() {
	int i,j,y;
	for (i = 0; i < 243; i++) {
		putchar('1');
		int num = i;
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
				putchar('-');
				putchar('0' + j);
			}
			num = num/3;
		}
		putchar('\n');
	}
}
