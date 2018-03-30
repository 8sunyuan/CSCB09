#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	int i,j,y;
	for (i = 0; i < 243; i++) {
		char curr_str[12] = {'1','\0'};
        char whole_str[12] = {'1','\0'};
        char letter[2];
        letter[1] = '\0';
        int curr_num;
        int add = 1;
        int sum = 0;
		int num = i;
		for (j = 2; j <= 6; j++) {
			y = num % 3;
            letter[0] = j + '0';
			if (y == 0) {
                strcat(curr_str, letter);
                strcat(whole_str, letter);
				putchar('0' + j);
			}
			else if (y == 1) {
                curr_num = atoi (curr_str);
                if (add) {
                    sum = sum + curr_num;
                } else {
                    sum = sum - curr_num;
                }
                curr_str[0] = '\0';
                strcat(curr_str, letter);
                strcat(whole_str, "+");
                strcat(whole_str, letter);
                add = 1;
			}
			else {
                curr_num = atoi (curr_str);
                if (add) {
                    sum = sum + curr_num;
                } else {
                    sum = sum - curr_num;
                }
                curr_str[0] = '\0';
                strcat(curr_str, letter);
                add = 0;

                strcat(whole_str, "-");
                strcat(whole_str, letter);
			}
			num = num/3;
		}
        if (sum == 47){
            printf("%s", whole_str);
            printf("\n");
        }
	}
}
