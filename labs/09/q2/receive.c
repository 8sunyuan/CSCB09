#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

static int ch = 0;
static int i = 0;

void getone() {
		ch <<= 1;
		i++;
	}

void gettwo() {
		ch <<= 1;
		ch++;
		i++;
	}

int main() {
	printf("My pid is %d\n", getpid());
	signal(SIGUSR1, getone);
	signal(SIGUSR2, gettwo);

	while (i < 8) {
		// lets wait for character mr rosenthal
	}

	printf("%c\n", ch);


	return 0;







}
