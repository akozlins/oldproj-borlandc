#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

int graf[20][20] = {0};

int find(short n, short last, short now, short fin)
{
   short i, k;
   graf[last][now] = 0;
   k=0;
//   printf("point %d\n",now+1);
   if(now == fin)
   {
      return 1;
   }
   if(now == n)
   {
      return 0;
   }
   for(i=0;i<n;i++)
   {
      if(i == last) continue;
      if(i == now) continue;
      if(graf[now][i] != 0)
      {
	       k = k | find(n, now, i, fin);
      }
   }
   return k;
}


int main(int argc, char **argv)
{
   int i,j,n,m;
   FILE *in;
   if(argc != 5)
   {
      printf("usage: %s links_file n_param from to\n", argv[0]);
      return 0;
   }
   in = fopen(argv[1],"rt");
   if(!in)
   {
      printf("cannot open file %s\n",argv[1]);
      return 0;
   }
   n=atof(argv[2]);
   for(i=0;i<n;i++)
   {
      graf[i][i]=1;
      for(j=0;j<n;j++)
      {
	 graf[i][j]=fgetc(in)-'0';
//	 printf("%d ", graf[i][j]);
      }
      fgetc(in);
//      printf("\n");
   }
   if(atof(argv[3])-1>n | atof(argv[3])-1<0)
   {
      printf("point %d not present\n", (int)(atof(argv[3])));
      close(in);
      return 0;
   }
   if(atof(argv[4])>n | atof(argv[4])<0)
   {
      printf("point %d not present\n", (int)(atof(argv[4])));
      close(in);
      return 0;
   }
   if(find(n,-1,atof(argv[3])-1,atof(argv[4])-1))
   {
      printf("yes\n");
   }
   else
   {
      printf("no\n");
   }
   fclose(in);
   return 1;
}