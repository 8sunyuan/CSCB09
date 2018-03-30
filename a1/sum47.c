#include <stdio.h>
#include <stdlib.h> 

int main() {
	int i,j,y;
	int num, add, curr, total;
	
	for (i = 0; i < 243; i++) {
		
		// Part I: Evaluate total for a given line
		//--------------------------------------------------------
		
		num = i;
        // Keeps track if current operation is add or subtract
		// add = 1 is add, add = 0 is subtract
        add = 1;
		// Current number we are building to add/subtract to total
        curr = 1;
        total = 0;

        for (j = 2; j <= 6; j++) {
			y = num % 3;
			// No operation, concatenate j value by adding to 10*current value
			// ex. 123 = 12*10 + 3
			if (y == 0) {
				curr = curr*10 + j;
			} 
            // Next operation is Addition
			else if (y == 1) {
				if (add) {
                    total = total + curr;
                    curr = j;
                } else {
                    total = total - curr;
                    curr = j;
                }
                add = 1;
			}
            // Next operation is Subtraction
			else {
				if (add) {
                    total = total + curr;
                    curr = j;
                } else {
                    total = total - curr;
                    curr = j;
                }
                add = 0;
			}
			num = num/3;
		}
		// Perform last operation where current value ends with a 6
        if (add) {
            total = total + curr;
            curr = j;
        } else {
            total = total - curr;
            curr = j;
        }

		
		// Part II: Evaluate total for a given line
		// --------------------------------------------------------
		// Exact same code from loop from leftside.c
		// Use putchar to print the lines that have total equals 47
        if (total == 47) {
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
					putchar('-');
					putchar('0' + j);
				}
				num = num/3;
			}
			printf("\n");
        }
	}
}
