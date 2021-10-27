#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <signal.h>
int lsh_ls(char **args);
int stat(const char *pathname, struct stat *statbuf);
void sigHandle(int sig);
//int lsh_mv(char **args);