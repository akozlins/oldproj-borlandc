
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

void dirlist(char *name, int l = 0)
{
  printf("%s\n", name);

  DIR* dir = opendir(name);
  if(dir == NULL)
  {
    printf("Unable to open directory %s.\n", dir);
    return;
  }

  char path[1024];
  struct dirent *dent;
  while((dent = readdir(dir)) != NULL)
  {
    sprintf(path, "%s/%s", name, dent->d_name);

    if(strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0) continue;

    struct stat statbuf;
    if(stat(path, &statbuf) == -1) {
      printf("There is no access to file or directory.\n");
      closedir(dir);
      return;
    }

    if((statbuf.st_mode & S_IFDIR) != 0)
    {
      dirlist(path, l + 1);
    }
    else
    {
      for(int i = 0; i < l; i++) printf("  ");
      printf("%s/\n", dent->d_name);
    }
  }

  closedir(dir);

  return;
}

int main(int argn, char **argv)
{
  if(argn != 2) {
    printf("Usage: %s <dir>.\n", argv[0]);
    return 1;
  }

  struct stat statbuf;
  if(stat(argv[1], &statbuf) == -1) {
    printf("There is no access to file or directory.\n");
    return 1;
  }

  if((statbuf.st_mode & S_IFDIR) == 0) {
    printf("Directory %s does not exist.\n", argv[1]);
    return 1;
  }

  dirlist(argv[1]);

  return 0;
}
