#include <stdio.h>
#include "gcd.h"

int main()
{
    int i;

    for (i = 0; i < 20; i++)
        printf("gcd of 12 and %d is %d\n", i, gcd(12, i));
    return(0);
}
