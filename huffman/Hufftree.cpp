#include "Hufftree.h"
#include "Filework.h"

void Tree::FillCodes(int nd, ulong cod, int size) {
    if(nd < n_laters) {
	    codes[nd].cod = cod;
	    codes[nd].size = size;
	    return;
    }
    FillCodes(nodes[nd].left, cod << 1, size + 1);
    FillCodes(nodes[nd].right, (cod << 1) | 1, size + 1);
}

void Tree::Sort() {
    static int way = 0;
    ulong temp1, temp2, temp;
    if(way == 0) {
        for(int i = 0;i < n_laters;i++) {
	        for(int j = n_laters - 1;j > i;j--) {
	            temp1 = nodes[index[j]].weight;
    	        temp2 = nodes[index[j-1]].weight;
    	        if(temp1 != 0) {
	    	        if(temp2 == 0) {
		                temp = index[j];
		                index[j] = index[j-1];
		                index[j-1] = temp;
    		        }
	    	        else {
		                if(temp1 < temp2) {
			                temp = index[j];
			                index[j] = index[j-1];
			                index[j-1] = temp;
    		            }
	    	        }
	            }
    	    }
        }
        way = 1;
    }
    else {
	    for(int j = n_nodes-1;j > first;j--) {
	        temp1 = nodes[index[j]].weight;
	        temp2 = nodes[index[j-1]].weight;
	        if(temp1 != 0) {
		        if(temp2 == 0) {
		            temp = index[j];
		            index[j] = index[j-1];
		            index[j-1] = temp;
		        }
		        else {
		            if(temp1 < temp2) {
			            temp = index[j];
			            index[j] = index[j-1];
			            index[j-1] = temp;
                    }
	            }
	        }
        }
    }
}

Tree::Tree() {
    int i;
    for(i = 0;i < n_nodes;i++) {
	    index[i] = i;
    }
    for(i = 0;i < n_nodes;i++) {
	    nodes[i].left = -1;
	    nodes[i].right = -1;
	    nodes[i].up = -1;
	    nodes[i].weight = 0;
    }
    for(i = 0;i < n_laters;i++) {
	    codes[i].cod = 0;
	    codes[i].size = 0;
    }
    u_size = 0;
    c_size = 0;
    first = 0;
    last = 0;
    last_node = n_laters;
    return;
}

void Tree::FillTree(File *file) {
    int j;
    double counter = 1;
    int counter_temp = 0;
    int temp;
    printf("\tGathering statistic...\n");
    while(1) {
        temp = (int)(file->GetBits(n_bits));
        if(file->if_eof == true) break;
    	u_size++;
	    nodes[temp].weight++;
        if(counter == 1 || ((int)((100.0*counter)/file->statbuf.st_size)-1) == counter_temp) {
            printf("\r");
            clreol();
            counter_temp = (int)((100.0*counter)/file->statbuf.st_size);
            printf("\tProgress: %d%", counter_temp);
        }
        counter++;
    }
    printf("\r\tProgress: 100%\n");
    file->Rewind();
    Sort();
    while(nodes[index[last]].weight != 0) {
    	last++;
    }
    if((last - first) == 1) {
	    nodes[last_node].weight = nodes[index[first]].weight;
	    nodes[last_node].left = index[first];
	    nodes[last_node].right = index[first];
	    nodes[index[first]].up = last_node;
	    last_node++;
    }
    while(last - first != 1) {
	    nodes[last_node].weight = nodes[index[first]].weight + nodes[index[first+1]].weight;
	    nodes[last_node].left = index[first];
	    nodes[last_node].right = index[first+1];
	    nodes[index[first]].up = last_node;
	    nodes[index[first+1]].up = last_node;
	    last_node++;
	    first = first + 2;
	    last++;
        Sort();
    }
    FillCodes(last_node-1, 0, 0);
    c_size = 32*3 + 16*4 + n_bits
        + 8*(strlen(file->drive) + strlen(file->dir)
        + strlen(file->name) + strlen(file->ext))
        + 2*(n_bits + 1)*(last_node - n_laters);
    for(j = 0;j < n_laters;j++) {
	    c_size += codes[j].size * nodes[j].weight;
    }
    c_size = (c_size + 7)/8;
    return;
}

Tree::~Tree() {
}

void Tree::PrintInfo() {
    printf("N\tweight\tcod\tsize\n");
    for(int i = 0;i < n_laters;i++) {
        printf("%d\t%d\t0x%X\t%d\n", i, nodes[i].weight, codes[i].cod, codes[i].size);
        if(i % 16 == 0 && i != 0) getch();
    }
}