#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int n;
    if (argc != 2 || (n = atoi(argv[1])) <= 0)
	{
        fprintf(stderr, "usage: %s n\nn must be a positive integer\n", argv[0]);
        return(1);
    }
	else if (argc == 2)
	{
		// Change to n != 1 if you want to output the 1
		while (n != 2)
		{
			// Divide even number by 2
			if (n % 2 == 0)
			{
				n = n/2;
				printf("%d\n", n);
			}
			// Multiple odd number by 3 and add 1
			else
			{
				n = n*3 + 1;
				printf("%d\n", n);
			}
		}
	}
}
