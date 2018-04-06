#include <stdio.h>
#include <stdlib.h>

int main()
{
    char s[100];

    if (fgets(s, 100, stdin) != NULL);
        if (atoi(s) == 12)
            printf("do something\n");
    printf("in any case, do something else\n");
    return(0);
}
