#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
	int status = 0;
	long max_size = 0;
	char pathname[1000];
	memset(pathname, '\0', 1000);
	extern int maxfile(char *file, char *pathname, long *max_size);
	
	if (argc == 1) {
		status = maxfile(".", pathname, &max_size);
	} else {
		for (int i = 1; i < argc; i++) {
			if (maxfile(argv[i], pathname, &max_size)) {
				//perror(argv[i]);
				status = 1;
				break;
			}
		}
	}
	if (status == 0) printf("%s\n", pathname);
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
		
		// Check char array boundaries
		if (strlen(file) + strlen(p->d_name) > 998) {
			fprintf(stderr, "Exceeded array boundaries\n");
			closedir(dp);
			return 1;
		}
		
		memset(temp, '\0', 1000);
		strcpy(temp, file);
		strcat(temp, "/");
		strcat(temp, p->d_name);
			
		if (lstat(temp, &statbuf)) {
			perror(temp);
			closedir(dp);
			return 1;
		} else {
			// Base Case: Regular file, compare size
			if (S_ISREG(statbuf.st_mode)) {
				if (statbuf.st_size > *max_size) {
					*max_size = statbuf.st_size;
					strcpy(pathname, temp); 
				}
			// Recursion: Subdirectory, get largest file of directory
			} else if (S_ISDIR(statbuf.st_mode)) {
				if (maxfile(temp, pathname, max_size))
					return 1;
			}
		}
	}
	closedir(dp);
	return 0;
}
