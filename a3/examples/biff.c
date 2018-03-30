/*
 * A "biff"-like program for telling you that you have new mail.
 * It takes advantage of being run under "myinit" by exiting when the going
 * gets tough.  Ok, it's not very tough to deal with the file shrinking, but
 * it makes this more useful as a demo program in assignment three.
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAIL "/var/mail"

static char *progname;


int main(int argc, char **argv)
{
    /* some error-checking functions which always succeed: */
    extern FILE *efopen(char *file, char *mode);
    extern char *ecat(char *s, char *t);
    extern char *egetenv(char *varname);
    extern long filesize(FILE *fp);  /* note: loses current file position */
    extern void efseek(FILE *fp, long pos);
    extern void showfrom(FILE *fp);  /* show From line info */

    FILE *fp;
    long oldsize, newsize;

    progname = argv[0];
    fp = efopen(ecat(MAIL "/", egetenv("USER")), "r");
    oldsize = filesize(fp);

    while (1) {
	sleep(3);  /* several minutes would be more reasonable, but you'll
		      want it short to be able to observe its behaviour */
	if ((newsize = filesize(fp)) == oldsize) {
	    /* no new mail; just sleep */
	} else if (newsize > oldsize) {
	    efseek(fp, oldsize);
	    printf("New mail from ");
	    showfrom(fp);
	    oldsize = newsize;
	} else {
	    printf("File shrunk -- exiting\n");
	    exit(0);
	}
    }
}


FILE *efopen(char *file, char *mode)
{
    FILE *fp;

    if ((fp = fopen(file, mode)) == NULL) {
	perror(file);
	    /* pedagogical note: try changing "/var/mail" to a typo such as
	     * "/vra/mail" and you'll see why we insist that the exact file
	     * name is passed to perror() rather than a string such as "your
	     * mailbox"...
	     */
	exit(1);
    }

    return(fp);
}


void showfrom(FILE *fp)  /* show From line info */
{
    int c = getc(fp);
    while (c != EOF && !isspace(c) && c != '\n')
	c = getc(fp);
    while (c != EOF && c != '\n') {
	c = getc(fp);
	putchar(c);
    }
}


char *ecat(char *s, char *t)
{
    static char buf[200];
    if (strlen(s) + strlen(t) + 1 > sizeof buf) {
	fprintf(stderr, "%s: string value is too big\n", progname);
	exit(1);
    }
    strcpy(buf, s);
    strcat(buf, t);
    return(buf);
}


char *egetenv(char *varname)
{
    char *p;

    if ((p = getenv(varname)) == NULL) {
	fprintf(stderr, "%s: %s variable not set in environment\n",
		progname, varname);
	exit(1);
    }

    return(p);
}


void efseek(FILE *fp, long pos)
{
    if (fseek(fp, pos, SEEK_SET)) {
	perror("fseek()");
	exit(1);
    }
}


long filesize(FILE *fp)  /* note: loses current file position */
{
    if (fseek(fp, 0L, SEEK_END)) {
	perror("fseek()");
	exit(1);
    }
    return(ftell(fp));
}
