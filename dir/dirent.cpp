#include <dirent.h>
#include <sys\stat.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>

void printing(char *dirname) {
    printf("%s\n", dirname);
    static int i = 0;
    i++;
    char path[100];
    DIR *dir;
    struct dirent *dent;
    struct stat statbuf;
    if((dir = opendir(dirname)) == NULL) {
	printf("Unable to open directory %s.\n", dir);
	return;
    }
    while((dent = readdir(dir)) != NULL) {
	strcpy(path, dirname);
	strcat(path, "\\");
	strcat(path, dent->d_name);
	if(!strcmp(dent->d_name, ".") || !strcmp(dent->d_name, "..")) continue;
	if(stat(path, &statbuf) == -1) {
	    printf("There is no access to file or directory.\n");
	    closedir(dir);
	    return;
	}
	if((statbuf.st_mode & S_IFDIR) != 0) {
	    printing(path);
	}
	else {
	    for(int j = 1;j<i;j++) {
		printf("\t");
	    }
	    printf("%s\n", dent->d_name);
	}
    }
    closedir(dir);
//    getch();
    i--;
    return;
}

int main(int argn, char **argv) {
    DIR *dir;
    struct dirent *dent;
    struct stat statbuf;
    if(argn != 2) {
	printf("Usage: %s directory_name.\n", argv[0]);
	return 1;
    }
    if(stat(argv[1], &statbuf) == -1) {
	printf("There is no access to file or directory.\n");
	return 1;
    }
    if((statbuf.st_mode & S_IFDIR) == 0) {
	printf("Directory %s does not exist.\n", argv[1]);
	return 1;
    }
    printing(argv[1]);
    return 0;
}