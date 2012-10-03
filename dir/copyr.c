#include <stdio.h>
#define N 1000
int main(int argn, char **argc)
{
	FILE *from;
	FILE *to;
	char a[N];
	int i;
	int j;
	int num;
	if(argn != 3)
	{
	printf("usage: %s file_from file_to\n", argc[0]);
	return 1;
	}
	from = fopen(argc[1], "rb");
	to = fopen(argc[2], "wb");
	if(!from)
	{
	printf("cannot open %s file\n", argc[1]);
	return 1;
	}
	if(!to)
	{
	printf("cannot creat %s file\n", argc[2]);
	fclose(from);
	return 1;
	}
	while(!feof(from))
	{
	num = fread(a, 1, N, from);
	fwrite(a, 1, num, to);
	}
	fclose(from);
	fclose(to);
	return 0;
}