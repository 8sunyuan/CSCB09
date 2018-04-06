#include "gcd.h"

int gcd(int x, int y)
{
    int t;

    while (y) {
        t = x;
        x = y;
        y = t % y;
    }
    return(x);
}
