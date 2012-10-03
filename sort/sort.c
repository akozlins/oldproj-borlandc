#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

static int N;

void del(struct tree *);
struct tree
{
   char *elem;
   long freq;
   struct tree *left;
   struct tree *right;
};

struct tree *tr = NULL;

int add(char *elem)
{
   struct tree *temp = NULL;
   if(tr==NULL)
   {
      tr = (struct tree *)malloc(sizeof(struct tree));
      if(tr == NULL)
      {
	 printf("not enough memory to allocate buffer\n");
	 return 0;
      }
      tr->left = NULL;
      tr->right = NULL;
      tr->elem = NULL;
      tr->elem = (char *)malloc(strlen(elem)+1);
      if(tr->elem == NULL)
      {
	 printf("not enough memory to allocate buffer\n");
	 free(tr);
	 tr = NULL;
	 return 0;
      }
      strcpy(tr->elem, elem);
      tr->freq = 1;
      N = 1;
      return 1;
   }
   temp = tr;
   while(1)
   {
      if(strcmp(temp->elem, elem) == 0)
      {
	 temp->freq++;
	 return 1;
      }
      else
      if(strcmp(temp->elem, elem) < 0)
      {
	 if(temp->left == NULL)
	 {
	    temp->left = (struct tree *)malloc(sizeof(struct tree));
	    if(temp->left == NULL)
	    {
	       printf("not enough memory to allocate buffer\n");
	       del(tr);
	       return 0;
	    }
	    temp = temp->left;
	    temp->left = NULL;
	    temp->right = NULL;
	    temp->elem = NULL;
	    temp->elem = (char *)malloc(strlen(elem)+1);
	    if(temp->elem == NULL)
	    {
	       printf("not enough memory to allocate buffer\n");
	       del(tr);
	       return 0;
	    }
	    strcpy(temp->elem, elem);
	    temp->freq = 1;
	    N++;
	    return 1;
	 }
	 temp = temp->left;
      }
      else
      if(strcmp(temp->elem, elem) > 0)
      {
	 if(temp->right == NULL)
	 {
	    temp->right = (struct tree *)malloc(sizeof(struct tree));
	    if(temp->right == NULL)
	    {
	       printf("not enough memory to allocate buffer\n");
	       del(tr);
	       return 0;
	    }
	    temp = temp->right;
	    temp->left = NULL;
	    temp->right = NULL;
	    temp->elem = NULL;
	    temp->elem = (char *)malloc(strlen(elem)+1);
	    if(temp->elem == NULL)
	    {
	       printf("not enough memory to allocate buffer\n");
	       del(tr);
	       return 0;
	    }
	    strcpy(temp->elem, elem);
	    temp->freq = 1;
	    N++;
	    return 1;
	 }
	 temp = temp->right;
      }
   }
}

void del(struct tree *temp)
{
   if(temp == NULL)
   {
      return;
   }
   del(temp->left);
   temp->left = NULL;
   del(temp->right);
   temp->right = NULL;
   if(temp->elem != NULL)
   {
      free(temp->elem);
      temp->elem = NULL;
   }
   free(temp);
   temp = NULL;
   return;
}

void prin(struct tree *temp)
{
   if(temp == NULL)
   {
      return;
   }
   prin(temp->right);
   printf("%s\t%d\n",temp->elem,temp->freq);
   prin(temp->left);
   return;
}

void fprin(struct tree *temp, FILE *out)
{
   if(temp == NULL)
   {
      return;
   }
   fprin(temp->right,out);
   fprintf(out,"%s\t%d\n",temp->elem,temp->freq);
   fprin(temp->left,out);
   return;
}

int main(int argc, char **argv)
{
   char strin[1000];
   FILE *in, *out;
   in = fopen(argv[1],"rb");
   if(argc != 3)
   {
      printf("usage: %s file_in file_out",argv[0]);
      return 0;
   }
   if(!in)
   {
      printf("cannot open file %s\n",argv[1]);
      return 0;
   }
   out = fopen(argv[2],"wb");
   if(!out)
   {
      printf("cannot open file %s\n",argv[2]);
      fclose(in);
      return 0;
   }
   while(!feof(in))
   {
      fscanf(in,"%s",strin);
      if(!add(strin))
      {
	 del(tr);
	 return 0;
      }
   }
   fprin(tr,out);
   del(tr);
   fclose(in);
   fclose(out);
   return 1;
}