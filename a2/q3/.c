#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
// struct file{
// 	int size;
// 	char *pathname;
// }

int main(int argc, char **argv) {
	int status = 0;
	long max_size = 0;;
	struct stat statbuf;
	char pathname[1000];
	memset(pathname, '\0', 1000);
	
	extern void maxfile(char *file, char *pathname, off_t *max_size);

	for (int i = 1; i < argc; i++) {
		if (lstat(argv[i], &statbuf)) {
			perror(argv[i]);
			status = 1;
			break;
		} else {
			// Find largest file 
			if (S_ISREG(statbuf.st_mode)) {
				if (statbuf.st_size > *max_size) {
					*max_size = statbuf.st_size;
					strcpy(pathname, argv[i]);
				}
			// Perform recursion maxfile on directory
			} else if (S_ISDIR(statbuf.st_mode)) {
				if (maxfile(argv[i], pathname, &max_size))
					status = 1;
			}
		}
	}
	printf("%s\n", pathname);
	return status;
}

int maxfile(char *file, char *pathname, long *max_size) {
	
	struct stat statbuf;
	DIR *dp;
	struct dirent *p;
	char temp[1000];

	if ((dp = opendir(file)) == NULL) {
	 	perror(file);
	 	return(1);
     }
    // Check size of each file in current directory
    while ((p = readdir(dp))) {
		
		// Ignore current and parent directory to avoid infinite loop
		if (p->d_name[0] == '.')
			if (p->d_name[1] == '\0' || p->d_name[1] == '.')
				continue;
		
		memset(temp, '\0', 1000);
		strcpy(temp, file);
		strcat(temp, "/");
		strcat(temp, p->d_name);
			
		if (lstat(p, &statbuf)) {
			perror(p);
			*status = 1;
		} else {
			// Base Case: Regular file, compare size
			if (S_ISREG(statbuf.st_mode)) {
				if (statbuf.st_size > *max_size) {
					*max_size = statbuf.st_size;
					strcpy(pathname, p->d_name); 
				}
			// Recursion: Subdirectory, get largest file of directory
			} else if (S_ISDIR(statbuf.st_mode)) {
				if (maxfile(p->d_name, pathname, max_size))
					return 1;
			}
		}
	}
	closedir(dp);
	return 0;
}
