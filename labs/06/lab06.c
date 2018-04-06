#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

int main(int argc, char* argv[]) {
	struct stat statbuf1;
	struct stat statbuf2;
	if (argc != 3) {
		fprintf(stderr, "usage: %s file1 file2\n", argv[0]);
		return(1);
	}

	if (stat(argv[1], &statbuf1) || stat(argv[2], &statbuf2)) {
		perror("stat");
		return(1);
	}
	
	if (statbuf1.st_mtime - statbuf2.st_mtime > 0) ? printf("%s\n", argv[1]) : printf("%s\n", argv[2]);
	return 0;
}
