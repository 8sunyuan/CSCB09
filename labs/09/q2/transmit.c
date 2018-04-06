#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>

int main(int argc, char **argv) {
	if (argc != 3) {
		fprintf(stderr, "usage: %s pid char", argv[0]);
		return (1);
	}

	for (int i = 0; i < 8; i++) {
		int sig = (argv[2][0] & (1 << (7-i))) > 0;
		int send = sig ? SIGUSR2 : SIGUSR1;
		kill(atoi(argv[1]), send);
		usleep(1000);
	}
	return 0;
}