#include "Huff.h"
#include "Hufftree.h"
#include "Filework.h"

int Check(char **argv) {
   if(!strcmp(argv[1], "-x") || !strcmp(argv[1], "--extract")) {
	    return _extract;
    }
    if(!strcmp(argv[1], "-v") || !strcmp(argv[1], "--view")) {
    	return _view;
    }
	if(!strcmp(argv[1], "-r") || !strcmp(argv[1], "--remove")) {
	    return _remove;
	}
    if(!strcmp(argv[1], "-a") || !strcmp(argv[1], "--add")) {
		return _add;
    }
    printf("Huff: You must specify one of the `-xavr' options.\n");
    printf("Try '%s --help' for more information.\n", argv[0]);
    return _error;
}

void Help() {
    printf("-r, --remove: ");
    printf("remove file from the archive.\n");
    printf("-a, --add: ");
    printf("add file to the end of an archive.\n");
    printf("-v, --view: ");
    printf("list the contents of an archive.\n");
    printf("-x, --extract: ");
    printf("extract files from an archive.\n");
}

void Add(char *file, char *archive) {
    double counter = 1;
    int counter_temp = 0;
    File in(file, mode_read);
    char default_archive[MAXFILE+MAXEXT-1];
    strcpy(default_archive, in.name);
    strcat(default_archive, ".arc");
    if(archive == NULL) {
        archive = default_archive;
    }
    File out(archive, mode_append);
    int temp;
    Tree tree;
    printf("Creating tree...\n");
    tree.FillTree(&in);
    printf("Wrinting header to file...\n");
    out.PutChar(0);
    out.PutBits(ar_id, 24);

    out.PutChar(strlen(in.drive));
    out.Write(in.drive, strlen(in.drive) + 1);
    out.PutChar(strlen(in.dir));
    out.Write(in.dir, strlen(in.dir) + 1);
    out.PutChar(strlen(in.name));
    out.Write(in.name, strlen(in.name) + 1);
    out.PutChar(strlen(in.ext));
    out.Write(in.ext, strlen(in.ext) + 1);

    out.PutBits(tree.u_size, 32);
    out.PutBits(tree.c_size, 32);
    out.PutBits(tree.last_node-n_laters, n_bits);
    for(int i = n_laters;i < tree.last_node;i++) {
        out.PutBits(tree.nodes[i].left, n_bits+1);
	    out.PutBits(tree.nodes[i].right, n_bits+1);
    }
    printf("Coding...\n");
    while(1) {
        temp = (int)(in.GetBits(n_bits));
        if(in.if_eof == true) break;
	    out.PutBits(tree.codes[temp].cod, tree.codes[temp].size);
        if(counter == 1 || ((int)((100.0*counter)/tree.u_size)-1) == counter_temp) {
            printf("\r");
            clreol();
            counter_temp = (int)((100.0*counter)/tree.u_size);
            printf("\tProgress: %d%", counter_temp);
        }
        counter++;
    }
    printf("\r\tProgress: 100%\n");
    printf("Compression complete.\n");
}

void Decode(char *file, char *archive) {
	double counter;
    int counter_temp = 1;
    File arc(archive, mode_read);
    File out;
    Tree tree;
    char path[MAXPATH];
    char drive[MAXDRIVE];
    char dir[MAXDIR];
    char name[MAXFILE];
    char ext[MAXEXT];
    int drive_size;
    int dir_size;
    int name_size;
    int ext_size;
    ulong j, temp, now;
    while(arc.GetChar() != EOF) {
	    if(arc.GetBits(24) != (ulong)(ar_id)) {
	        printf("File %s is not arhive or damaged.\n", archive);
            throw "File is not arhive or damaged.";
	    }
        drive_size = arc.GetChar();
    	arc.Read(drive, drive_size+1);
        dir_size = arc.GetChar();
    	arc.Read(dir, dir_size + 1);
        name_size = arc.GetChar();
    	arc.Read(name, name_size + 1);
        ext_size = arc.GetChar();
    	arc.Read(ext, ext_size + 1);
        fnmerge(path, drive, dir, name, ext);
    	strlwr(path);                  //!!!
	    if((file != NULL) && strcmp(path, file)) {
            strlwr(file);              //!!!
	        while(1) {
    		    arc.GetBits(32);
	    	    arc.Seek(arc.GetBits(32) - drive_size
                    - dir_size - name_size - ext_size
                    - 21);
		        if(arc.GetChar() == EOF) {
		            printf("Unable to find file %s in archive.\n", file);
                    return;
		        }
		        if(arc.GetBits(24) != ar_id) {
		            printf("File %s is not archive or damaged.\n", archive);
                    throw "File is not archive or damaged.";
		        }
                drive_size = arc.GetChar();
    	        arc.Read(drive, drive_size + 1);
                dir_size = arc.GetChar();
    	        arc.Read(dir, dir_size + 1);
                name_size = arc.GetChar();
    	        arc.Read(name, name_size + 1);
                ext_size = arc.GetChar();
            	arc.Read(ext, ext_size + 1);
                strlwr(file);                  //!!!
                fnmerge(path, drive, dir, name, ext);
                strlwr(path);                  //!!!
		        if(!strcmp(path, file)) break;
    	    }
        }
        out.Open(path, mode_write);        //!!!
        tree.u_size = arc.GetBits(32);
        tree.c_size = arc.GetBits(32);
        tree.last_node = n_laters + arc.GetBits(n_bits);
        for(int i = n_laters;i < tree.last_node;i++) {
            tree.nodes[i].left = arc.GetBits(n_bits + 1);
            tree.nodes[i].right = arc.GetBits(n_bits + 1);
        }
        now = tree.last_node - 1;
        j = 0;
        counter = 1;
        printf("File %s is being decoded...\n", path);
        while(j < tree.u_size) {
            temp = arc.GetBit();
            if(temp == 1) {
           		now = tree.nodes[now].right;
       	    }
    	    else {
	       	    now = tree.nodes[now].left;
	        }
    	    if(now < n_laters) {
	      	    out.PutBits(now, n_bits);
        	    now = tree.last_node - 1;
        	    j++;
            	if(counter == 1 || ((int)((100.0*counter)/tree.u_size)-1) == counter_temp) {
            		printf("\r");
            		clreol();
        	    	counter_temp = (int)((100.0*counter)/tree.u_size);
    	        	printf("\tProgress: %d%", counter_temp);
	        	}
	            counter++;
    	    }
        }
	    printf("\r\tProgress: 100%\n");
    	if(file != NULL) {
    	    break;
	    }
        arc.NewLater();
        printf("::::::::::::::::%X\n", arc.GetChar());
    }
    printf("Decoding complete.\n");
}

void List(char *archive) {
    File fl(archive, mode_read);
    ulong c_size;
    ulong u_size;
    char path[MAXPATH];
    char drive[MAXDRIVE];
    char dir[MAXDIR];
    char name[MAXFILE];
    char ext[MAXEXT];
    int drive_size;
    int dir_size;
    int name_size;
    int ext_size;
    while(fl.GetChar() != EOF) {
	    if(fl.GetBits(24) != ar_id) {
	        printf("File %s is not archive or damaged.\n", archive);
            throw "File is not archive or damaged.";
	    }
        drive_size = fl.GetChar();
  	    fl.Read(drive, drive_size + 1);
        dir_size = fl.GetChar();
       	fl.Read(dir, dir_size + 1);
        name_size = fl.GetChar();
       	fl.Read(name, name_size + 1);
        ext_size = fl.GetChar();
       	fl.Read(ext, ext_size + 1);
	    u_size = fl.GetBits(32);
	    c_size = fl.GetBits(32);
   	    fl.Seek(c_size - drive_size - dir_size
            - name_size - ext_size - 21);
        fnmerge(path, drive, dir, name, ext);
	    printf("%s\t%.0f%\n", path, 100.0*c_size/u_size);
    }
}

void Delete(char *archive, char *file) {
    File in(archive, mode_read);
    File out("temp.tmp", mode_write);
    ulong c_size;
    ulong u_size;
    ulong i;
    int k = 0;
    char path[MAXPATH];
    char drive[MAXDRIVE];
    char dir[MAXDIR];
    char name[MAXFILE];
    char ext[MAXEXT];
    int drive_size;
    int dir_size;
    int name_size;
    int ext_size;
    while(in.GetChar() != EOF) {
	    if(in.GetBits(24) != ar_id) {
	        printf("File %s is not archive or damaged.\n", archive);
    	    remove("temp.tmp");
            throw "File is not archive or damaged.";
    	}
        drive_size = in.GetChar();
        in.Read(drive, drive_size + 1);
        dir_size = in.GetChar();
       	in.Read(dir, dir_size + 1);
        name_size = in.GetChar();
       	in.Read(name, name_size + 1);
        ext_size = in.GetChar();
       	in.Read(ext, ext_size + 1);
	    u_size = in.GetBits(32);
    	c_size = in.GetBits(32);
        fnmerge(path, drive, dir, name, ext);
    	if(strcmp(path, file)) {
	        out.PutChar(0);
    	    out.PutBits(ar_id, 24);
            out.PutChar(drive_size);
    	    out.Write(drive, drive_size + 1);
            out.PutChar(dir_size);
    	    out.Write(dir, dir_size + 1);
            out.PutChar(name_size);
    	    out.Write(name, name_size + 1);
            out.PutChar(ext_size);
	        out.Write(ext, ext_size + 1);
            out.PutBits(u_size, 32);
            out.PutBits(c_size, 32);
	    }
        else {
   	        in.Seek(c_size - drive_size - dir_size
                - name_size - ext_size - 21);
    	    k = 1;
	        break;
	    }
    	for(i = 0;i < (c_size - drive_size - dir_size - name_size - ext_size - 21);i++) {
    	    out.PutChar(in.GetChar());
    	}
    }
    if(k == 0) {
    	printf("There are not file %s in arhive.\n", file);
        out.~File();
    	remove("temp.tmp");
    	return;
    }
    while((k = in.GetChar()) != EOF) {
    	out.PutChar(k);
    }
    in.Open("temp.tmp", mode_read);
    out.Open(archive, mode_write);
    if((k = in.GetChar()) == EOF) {
    	printf("Arhive %s is empty.", archive);
        in.~File();
    	out.~File();
    	remove(archive);
        remove("temp.tmp");
        return;
    }
    while((k = in.GetChar()) != EOF) {
	    out.PutChar(k);
    }
    in.~File();
    remove("temp.tmp");
}
