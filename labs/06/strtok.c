#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    char *p;

    if (argc != 2) {
	fprintf(stderr, "usage: %s string\n", argv[0]);
	return(1);
    }

    for (p = strtok(argv[1], " \t\n"); p; p = strtok(NULL, " \t\n"))
	printf("I see a token '%s'\n", p);
    return(0);
}

/*
Notes about strtok():
    - the first call gives it the string to pull apart; subsequent calls give
      it a NULL first pointer to mean to continue processing the same string,
      or a new string as first argument to switch to a new string.
    - including "\n" in the list of whitespace characters saves us from having
      to remove the \n at the end of the string which fgets() gives us (not an
      issue above, but is an issue for assignment two)
*/
