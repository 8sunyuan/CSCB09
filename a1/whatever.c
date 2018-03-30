#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

int main() {
	int i,j,y;
	for (i = 0; i < 243; i++) {
		//putchar('1');
		int num = i;
        char string[12];
        //
        int add = 1;
        int c = 1;
        int p = 0;
		//

        for (j = 2; j <= 6; j++) {
			y = num % 3;
			if (y == 0) {
				c = c*10 + j;
			} 
            // Addition
			else if (y == 1) {
				if (add) {
                    p = p + c;
                    c = j;
                } else {
                    p = p - c;
                    c = j;
                }
                add = 1;
			}
            //Subtraction
			else {
				if (add) {
                    p = p + c;
                    c = j;
                } else {
                    p = p - c;
                    c = j;
                }
                add = 0;
			}
			num = num/3;
		}
        if (add) {
            p = p + c;
            c = j;
        } else {
            p = p - c;
            c = j;
        }

        //printf("%d\n", p);

        if (p == 47) {
            printf("Hello\n");
        }

		//printf("\n");
	}
}