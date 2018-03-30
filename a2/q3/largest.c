#include<stdio.h>
#include<string.h>
#include <dirent.h>
#include <sys/stat.h>

int helper(char*, long*, char*);

int main(int argc, char* argv[]) {
  long largest = 0;
  char path[1001];
  memset(path, '\0', 1001);

  if (argc == 1) {
    helper(".", &largest, path);
  } else {
    for (int i = 1; i < argc; i++) {
      if (helper(argv[i], &largest, path) < 0) {
        fprintf(stderr, "Error opening directory\n");
        return -1;
      }
    }
  }
  printf("%s\n", path);
  return 0;
}

int helper(char* path, long* largest, char* lrgpath) {
    struct dirent *entry;
    DIR* dir;
    char store[1001];

    if ((dir = opendir(path)) == NULL) {
      return -1;
    }

    struct stat stat_buf;
    while ((entry = readdir(dir)) != NULL) {

      if (entry->d_name[0]=='.'){
        if (entry->d_name[1] == '\0' || entry->d_name[1] == '.') {
          continue;
        }
      }
      memset(store, '\0', 1001);
      strcpy(store, path);
      strcat(store,"/");
      strcat(store,entry->d_name);

      lstat(store, &stat_buf);

      if (S_ISREG(stat_buf.st_mode)) {
        if (stat_buf.st_size > *largest) {
          *largest = stat_buf.st_size;
          strcpy(lrgpath, store);
        }
      } else if (S_ISDIR(stat_buf.st_mode)) {
        if (helper(store, largest, lrgpath) < 0) {
          return -1;
        }
      }
    }
    closedir(dir);
    return 0;
}
