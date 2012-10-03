#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define read 0
#define write 1
#define add 2

#define number_of_bits 8
#define number_of_laters 256
#define number_of_nodes 511

struct File {
    char file_name[20];
    FILE *fl;
    unsigned int later;
    unsigned long mask;
    int rw;
};

struct Tree {
    unsigned long u_size;
    unsigned long c_size;
    struct Cod {
	unsigned long cod;
	int size;
    } codes[number_of_laters];
    unsigned long index[number_of_nodes];
    struct Node {
	int left;
	int right;
	int up;
	unsigned long weight;
    } nodes[number_of_nodes];
    int last;
    int first;
    int last_node;
};

struct File *OpenInFile(char *file_name) {
    struct File *fl = (struct File *)(malloc(sizeof(struct File)));
    if(fl == NULL) return NULL;
    fl->fl = fopen(file_name,"rb");
    if(fl->fl == NULL) {
	free(fl);
	return NULL;
    }
    strcpy(fl->file_name, file_name);
    fl->rw = read;
    fl->mask = 0;
    fl->later = 0;
    return fl;
}

void CloseInFile(struct File *file) {
    if(file  == NULL) return;
    if(file->fl != NULL) fclose(file->fl);
    free(file);
    return;
}

struct File *OpenOutFile(char *file_name, int param) {
    struct File *fl = (struct File *)(malloc(sizeof(struct File)));
    if(fl == NULL) return NULL;
    if(param == add) {
	fl->fl = fopen(file_name,"ab");
	fl->rw = add;
    }
    else {
	fl->fl = fopen(file_name,"wb");
	fl->rw = write;
    }
    if(fl->fl == NULL) {
	free(fl);
	return NULL;
    }
    strcpy(fl->file_name, file_name);
    fl->mask = 0x80;
    fl->later = 0;
    return fl;
}

void CloseOutFile(struct File *file) {
    if(file  == NULL) return;
    if(file->fl == NULL) return;
    fputc(file->later, file->fl);
    fclose(file->fl);
    free(file);
    return;
}

unsigned int GetBit(struct File *file) {
    unsigned int temp = 0;
    if(file->mask == 0) {
	file->later = fgetc(file->fl);
	file->mask = 0x80;
    }
    temp = file->mask & file->later;
    file->mask = file->mask >> 1;
    if(temp > 0) return 1;
    return 0;
}

void PutBit(unsigned int bit, struct File *file) {
    if(file->mask == 0) {
	fputc(file->later, file->fl);
	file->later = 0;
	file->mask = 0x80;
    }
    if(bit == 1) file->later = file->later | file->mask;
    file->mask = file->mask >> 1;
    return;
}

unsigned long GetBits(int n, struct File *file) {
    int i;
    unsigned long temp = 0;
    for(i = 0;i < n;i++) {
	temp = temp << 1;
	temp = temp | GetBit(file);
    }
    return temp;
}

void PutBits(unsigned long bits, int n, struct File *file) {
    int i;
    unsigned long temp = 1;
    temp = temp << (n - 1);
    for(i = 0;i < n;i++) {
	if((temp & bits) > 0) {
	    PutBit(1, file);
	}
	else {
	    PutBit(0, file);
	}
	temp = temp >> 1;
    }
    return;
}

void TreeInit(struct Tree *tree) {
    int i;
    for(i = 0;i < number_of_nodes;i++) {
	tree->index[i] = i;
    }
    for(i = 0;i < number_of_nodes;i++) {
	tree->nodes[i].left = -1;
	tree->nodes[i].right = -1;
	tree->nodes[i].up = -1;
	tree->nodes[i].weight = 0;
    }
    for(i = 0;i < number_of_laters;i++) {
	tree->codes[i].cod = 0;
	tree->codes[i].size = 0;
    }
    tree->u_size = 0;
    tree->c_size = 0;
    tree->first = 0;
    tree->last = 0;
    tree->last_node = number_of_laters;
    return;
}

void FillCodes(struct Tree *tree, int nd, unsigned long cod, int size) {
    if(nd < number_of_laters) {
	tree->codes[nd].cod = cod;
	tree->codes[nd].size = size;
	return;
    }
    FillCodes(tree, tree->nodes[nd].left, cod << 1, size + 1);
    FillCodes(tree, tree->nodes[nd].right, (cod << 1) | 1, size + 1);
}

void FillTree(struct Tree *tree, struct File *file) {
    int i, j;
    unsigned long temp1, temp2;
    unsigned int temp = 0;
    while((temp = fgetc(file->fl)) != EOF) {
	tree->u_size++;
	tree->nodes[temp].weight++;
    }
    rewind(file->fl);
    for(i = 0;i < number_of_laters;i++) {
	for(j = number_of_laters - 1;j > i;j--) {
	    temp1 = tree->nodes[tree->index[j]].weight;
	    temp2 = tree->nodes[tree->index[j-1]].weight;
	    if(temp1 != 0) {
		if(temp2 == 0) {
		    temp = tree->index[j];
		    tree->index[j] = tree->index[j-1];
		    tree->index[j-1] = temp;
		}
		else {
		    if(temp1 < temp2) {
			temp = tree->index[j];
			tree->index[j] = tree->index[j-1];
			tree->index[j-1] = temp;
		    }
		}
	    }
	}
    }
    while(tree->nodes[tree->index[tree->last]].weight != 0) {
	tree->last++;
    }
    if(tree->last - tree->first == 1) {
	tree->nodes[tree->last_node].weight = tree->nodes[tree->index[tree->first]].weight;
	tree->nodes[tree->last_node].left = tree->index[tree->first];
	tree->nodes[tree->last_node].right = tree->index[tree->first];
	tree->nodes[tree->index[tree->first]].up = tree->last_node;
	tree->last_node++;
    }
    while(tree->last - tree->first != 1) {
	tree->nodes[tree->last_node].weight = tree->nodes[tree->index[tree->first]].weight + tree->nodes[tree->index[tree->first+1]].weight;
	tree->nodes[tree->last_node].left = tree->index[tree->first];
	tree->nodes[tree->last_node].right = tree->index[tree->first+1];
	tree->nodes[tree->index[tree->first]].up = tree->last_node;
	tree->nodes[tree->index[tree->first+1]].up = tree->last_node;
	tree->last_node++;
	tree->first = tree->first + 2;
	tree->last++;
	for(j = number_of_nodes-1;j > tree->first;j--) {
	    temp1 = tree->nodes[tree->index[j]].weight;
	    temp2 = tree->nodes[tree->index[j-1]].weight;
	    if(temp1 != 0) {
		if(temp2 == 0) {
		    temp = tree->index[j];
		    tree->index[j] = tree->index[j-1];
		    tree->index[j-1] = temp;
		}
		else {
		    if(temp1 < temp2) {
			temp = tree->index[j];
			tree->index[j] = tree->index[j-1];
			tree->index[j-1] = temp;
		    }
		}
	    }
	}
    }
    FillCodes(tree, tree->last_node-1, 0, 0);
    tree->c_size = 160 + 96 + number_of_bits + 2*(number_of_bits+1)*(tree->last_node-number_of_laters);
    for(i = 0;i < number_of_laters;i++) {
	tree->c_size += tree->codes[i].size * tree->nodes[i].weight;
    }
    tree->c_size = (tree->c_size + 7)/8;
    return;
}

void Encode(char *in_file, char *out_file, int param) {
    struct File *in;
    struct File *out;
    unsigned int temp, i;
    struct Tree tree;
    if((in = OpenInFile(in_file)) == NULL) {
	printf("File %s does not exist\n", in_file);
	return;
    }
    if((out = OpenOutFile(out_file, param)) == NULL) {
	printf("File %s cannot be create or does not exist\n", out_file);
	CloseInFile(in);
	return;
    }
    printf("Encoding: Wait a minute...\n");
    TreeInit(&tree);
    FillTree(&tree, in);
/*    for(i = 1; i < 257; i++) {
	printf("%u\t%u\n", i-1, tree.codes[i-1].size);
	if((i % 16) == 0) getch();
    }*/
    fputc(0, out->fl);
    fputc(0x12, out->fl);
    fputc(0x34, out->fl);
    fputc(0x56, out->fl);
    fwrite(in->file_name, 1, 20, out->fl);
    PutBits(tree.u_size, 32, out);
    PutBits(tree.c_size, 32, out);
    PutBits(tree.last_node-number_of_laters, number_of_bits, out);
    for(i = number_of_laters;i < tree.last_node;i++) {
	PutBits(tree.nodes[i].left, number_of_bits+1, out);
	PutBits(tree.nodes[i].right, number_of_bits+1, out);
    }
    while((temp = fgetc(in->fl)) != EOF) {
	PutBits(tree.codes[temp].cod, tree.codes[temp].size, out);
    }
    printf("Encoding complete: ");
    printf("%.0f%\n", 100.0*tree.c_size/tree.u_size);
    CloseOutFile(out);
    CloseInFile(in);
}

void Decode(char *file_in, char *file_name, int param) {
    struct File *in;
    struct File *out;
    unsigned long i;
    unsigned long now;
    unsigned long temp;
    struct Tree tree;
    char name[20];
    if((in = OpenInFile(file_in)) == NULL) {
	printf("File %s does not exist\n", file_in);
	return;
    }
    printf("Decoding: Wait a minute...\n");
    while(fgetc(in->fl) != EOF) {
	if(GetBits(24, in) != 0x123456) {
	    printf("File %s is not arhive or damaged\n", file_in);
	    CloseInFile(in);
	    return;
	}
	fread(name, 1, 20, in->fl);
	strlwr(name);
	strlwr(file_name);
	if((param == 1) && strcmp(name, file_name)) {
	    while(1) {
		GetBits(32, in);
		fseek(in->fl, GetBits(32, in) - 32, SEEK_CUR);
		if(fgetc(in->fl) == EOF) {
		    printf("Unable to findt file %s in arhive", file_name);
		    CloseInFile(in);
		    return;
		}
		if(GetBits(24, in) != 0x123456) {
		    printf("File %s is not arhive or damaged\n", file_name);
		    CloseInFile(in);
		    return;
		}
		fread(name, 1, 20, in->fl);
		strlwr(name);
		strlwr(file_name);
		if(!strcmp(name, file_name)) break;
	    }
	}
	if((out = OpenOutFile(name, write)) == NULL) {
	    printf("File %s cannot be create\n", name);
	    CloseInFile(in);
	    return;
	}
	TreeInit(&tree);
	tree.u_size = GetBits(32, in);
	tree.c_size = GetBits(32, in);
	tree.last_node = number_of_laters + GetBits(number_of_bits, in);
	for(i = number_of_laters;i < tree.last_node;i++) {
	    tree.nodes[i].left = GetBits(number_of_bits + 1, in);
	    tree.nodes[i].right = GetBits(number_of_bits + 1, in);
	}
	now = tree.last_node - 1;
	i = 0;
	while(i < tree.u_size) {
	    temp = GetBit(in);
	    if(temp == 1) {
		now = tree.nodes[now].right;
	    }
	    else {
		now = tree.nodes[now].left;
	    }
	    if(now < number_of_laters) {
		PutBits(now, number_of_bits, out);
		now = tree.last_node - 1;
		i++;
	    }
	}
	if(param == 1) {
	    CloseOutFile(out);
	    break;
	}
	CloseOutFile(out);
	in->mask = 0;
	in->later = 0;
    }
    printf("Decoding complete\n");
    CloseInFile(in);
}

void List(char *file_name) {
    struct File *in;
    unsigned long c_size;
    unsigned long u_size;
    char name[20];
    if((in = OpenInFile(file_name)) == NULL) {
	printf("File %s does not exist\n", file_name);
	return;
    }
    while(fgetc(in->fl) != EOF) {
	if(GetBits(24, in) != 0x123456) {
	    printf("File %s is not arhive or damaged\n", file_name);
	    CloseInFile(in);
	    return;
	}
	fread(name, 1, 20, in->fl);
	u_size = GetBits(32, in);
	c_size = GetBits(32, in);
	fseek(in->fl, c_size - 32, SEEK_CUR);
	printf("%s\t%.0f%\n", name, 100.0*c_size/u_size);
    }
}

void Delete(char *file_name, char *del_file_name) {
    struct File *in;
    struct File *out;
    unsigned long c_size;
    unsigned long u_size;
    unsigned long i;
    int k = 0;
    char name[20];
    if((in = OpenInFile(file_name)) == NULL) {
	printf("File %s does not exist\n", file_name);
	return;
    }
    if((out = OpenOutFile("temp.tmp", write)) == NULL) {
	printf("Temporary file %s cannot be create\n");
	CloseInFile(in);
	return;
    }
    while(fgetc(in->fl) != EOF) {
	if(GetBits(24, in) != 0x123456) {
	    printf("File %s is not arhive or damaged\n", file_name);
	    CloseOutFile(out);
	    CloseInFile(in);
	    remove("temp.tmp");
	    return;
	}
	fread(name, 1, 20, in->fl);
	u_size = GetBits(32, in);
	c_size = GetBits(32, in);
	if(strcmp(name, del_file_name)) {
	    fputc(0, out->fl);
	    fputc(0x12, out->fl);
	    fputc(0x34, out->fl);
	    fputc(0x56, out->fl);
	    fwrite(name, 1, 20, out->fl);
	    fputc(u_size >> 24, out->fl);
	    fputc((u_size >> 16)&0xff, out->fl);
	    fputc((u_size >> 8)&0xff, out->fl);
	    fputc(u_size&0xff, out->fl);
	    fputc(c_size >> 24, out->fl);
	    fputc((c_size >> 16)&0xff, out->fl);
	    fputc((c_size >> 8)&0xff, out->fl);
	    fputc(c_size&0xff, out->fl);
	}
	else {
	    fseek(in->fl, c_size - 32, SEEK_CUR);
	    k = 1;
	    break;
	}
	for(i = 0;i < c_size - 32;i++) {
	    fputc(fgetc(in->fl), out->fl);
	}
	in->mask = 0;
	in->later = 0;
    }
    if(k == 0) {
	printf("There are not file %s in arhive\n", del_file_name);
	CloseInFile(in);
	CloseOutFile(out);
	remove("temp.tmp");
	return;
    }
    while((k = fgetc(in->fl)) != EOF) {
	fputc(k, out->fl);
    }
    CloseInFile(in);
    fclose(out->fl);
    free(out);
    if((in = OpenInFile("temp.tmp")) == NULL) {
	printf("Temporary file was not created\n", file_name);
	return;
    }
    if((out = OpenOutFile(file_name, write)) == NULL) {
	printf("File %s does not exist\n", file_name);
	CloseInFile(in);
	remove("temp.tmp");
	return;
    }
    if((k = fgetc(in->fl)) == EOF) {
	printf("Arhive %s is empty", file_name);
        CloseInFile(in);
	fclose(out->fl);
	remove(file_name);
        remove("temp.tmp");
        return;
    }
    fputc(k, out->fl);
    while((k = fgetc(in->fl)) != EOF) {
	fputc(k, out->fl);
    }
    CloseInFile(in);
    fclose(out->fl);
    free(out);
    remove("temp.tmp");
}

int main(int argn, char **argv) {
    int param;
    if(argn == 1) {
	printf("Usage: %s param [file_in] [file_out]\n", argv[0]);
	printf("Try '%s --help' for more information.\n", argv[0]);
	return 1;
    }
    if(!strcmp(argv[1], "-x") || !strcmp(argv[1], "--extract")) {
	param = 1;
    }
    else {
	if(!strcmp(argv[1], "-c") || !strcmp(argv[1], "--create")) {
	    param = 2;
	}
	else {
	    if(!strcmp(argv[1], "-t") || !strcmp(argv[1], "--list")) {
		param = 3;
	    }
	    else {
		if(!strcmp(argv[1], "-d") || !strcmp(argv[1], "--delete")) {
		    param = 4;
		}
		else {
		    if(!strcmp(argv[1], "-r") || !strcmp(argv[1], "--append")) {
			param = 5;
		    }
		    else {
			if(!strcmp(argv[1], "--help")) {
			    printf("-c, --create\n");
			    printf("create a new archive\n");
			    printf("-d, --delete\n");
			    printf("delete file from the archive\n");
			    printf("-r, --append\n");
			    printf("append file to the end of an archive\n");
			    printf("-t, --list\n");
			    printf("list the contents of an archive\n");
			    printf("-x, --extract\n");
			    printf("extract files from an archive\n");
			    return 1;
			}
			else {
			    printf("Huff: You must specify one of the `-xctdr?' options\n");
			    printf("Try '%s --help' for more information.\n", argv[0]);
			    return 1;
			}
		    }
		}
	    }
	}
    }
    switch(param) {
	case 2: {
	    if(argn != 4) {
		printf("Usage: %s %s file [arhive]\n", argv[0], argv[1]);
		return 1;
	    }
	    Encode(argv[2], argv[3], write);
	    return 0;
	}
	case 1: {
	    if(argn != 4 && argn != 3) {
		printf("Usage: %s %s arhive [file]\n", argv[0], argv[1]);
		return 1;
	    }
	    if(argn == 4) Decode(argv[2], argv[3], 1);
	    if(argn == 3) Decode(argv[2], argv[2], 0);
	    return 0;
	}
	case 3: {
	    if(argn != 3) {
		printf("Usage: %s %s arhive\n", argv[0], argv[1]);
		return 1;
	    }
	    List(argv[2]);
	    return 0;
	}
	case 4: {
	    if(argn != 4) {
		printf("Usage: %s %s arhive file\n", argv[0], argv[1]);
		return 1;
	    }
	    Delete(argv[2], argv[3]);
	    return 0;
	}
	case 5: {
	    if(argn != 4) {
		printf("Usage: %s %s file arhive\n", argv[0], argv[1]);
		return 1;
	    }
	    Encode(argv[2], argv[3], add);
	    return 0;
	}
    }
    return 0;
}
