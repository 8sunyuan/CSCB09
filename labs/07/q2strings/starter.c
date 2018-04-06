#include <stdio.h>

int main()
{
	int i;
    char s[27];
    for (; i < 26; i++) {
    	s[i] = i + 65;
    }
    s[i] = '\0';
    printf("%s\n", s);
    return(0);
}
