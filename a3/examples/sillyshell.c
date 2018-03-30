/*
 * This shell-like program doesn't do much, mostly for security reasons.
 * That is to say, while fiddling with "listener" and your assignment three
 * solution and such, you don't want to be allowing people around the planet
 * to execute interesting commands on your UTSC linux account.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

extern void date(), fakewho(), fakemail(), help(), logout();

struct {
    char *cmd;
    void (*fn)();
} cmdlist[] = {
    { "date", date },
    { "who", fakewho },
    { "mail", fakemail },
    { "?", help },
    { "help", help },
    { "logout", logout },
};



int main()
{
    char buf[500], *p;
    int i;
    while (printf("$ "), fflush(stdout), fgets(buf, sizeof buf, stdin)) {
	if ((p = strchr(buf, '\n')))
	    *p = '\0';
	if ((p = strchr(buf, '\r')))
	    *p = '\0';
	for (i = 0; i < sizeof cmdlist / sizeof cmdlist[0]; i++)
	    if (strcmp(cmdlist[i].cmd, buf) == 0)
		(*cmdlist[i].fn)();
    }
    return(0);
}


void date()
{
    system("date");
}


void fakewho()
{
    printf("bill     ttyp1    Feb 26 18:28 (bill.microsoft.com)\n");
    printf("bill     ttyp2    Feb 26 18:29 (bill.microsoft.com)\n");
}


void fakemail()
{
    char buf[80];
    time_t now;

    printf("Message 1:\n");
    (void)time(&now);
    printf("From ajr %s", ctime(&now));
    printf("To: %s@utoronto.ca\n", getenv("USER"));
    printf("Subject: thanks\n");
    printf("\n");
    printf("Thanks for running my \"sillyshell\" program.\n");
    printf("\n");
    printf("regards,\n");
    printf("ajr\n");
    
    printf("\nDisposition: ");
    fflush(stdout);
    fgets(buf, sizeof buf, stdin);
    printf("Deleted.\nEnd of mailbox.\n");
}


void help()
{
    printf("Please type a command, such as date, who, mail, or logout,\n");
    printf("and I'll consider executing it.\n");
}


void logout()
{
    exit(0);
}
