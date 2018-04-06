#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	int result, i, status;
	int sum = 0;
	extern int compute(int index);

	for (i = 0; i < 5; i++) {
		result = fork();
		
		if (result == -1) {
			perror("fork:");
			exit(1);
		} else if (result == 0) { // child process
			sum = compute(i);
			exit(sum);	
		}	
	}

	for (sum = i = 0; i < 5; i++) {
		if (wait(&status) == -1) {
			perror("wait");
		} else {
			sum += WEXITSTATUS(status);
		}
	}
	printf("%d\n", sum);	
	return 0;
}

int compute(int index) {
	int total;
	for (int i = 0; i < 5; i++)
		total += 5*i;
	return total;
}
