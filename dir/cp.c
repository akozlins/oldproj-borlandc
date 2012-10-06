
#include <stdio.h>

/**
 * Copy file.
 * usage: cp <src> <dest>
 */
int main(int argn, char **argc)
{
  if(argn != 3)
  {
    printf("usage: %s <src> <dest>\n", argc[0]);
    return 1;
  }

  FILE* src = fopen(argc[1], "rb");
  if(!src)
  {
    printf("cannot open %s file\n", argc[1]);
    return 1;
  }

  FILE* dest = fopen(argc[2], "wb");
  if(!dest)
  {
    printf("cannot creat %s file\n", argc[2]);
    fclose(src);
    return 1;
  }

  char buf[1024];
  while(!feof(src))
  {
    size_t n = fread(buf, 1, 1024, src);
    if(n < 0 || ferror(src))
    {
      printf("error while reading %s file\n", argc[1]);
      continue;
    }
    if(fwrite(buf, 1, n, dest) != n || ferror(dest))
    {
      printf("error while reading %s file\n", argc[2]);
      continue;
    }
  }

  fclose(src);
  fclose(dest);

  return 0;
}
