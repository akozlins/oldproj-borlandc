#include "Filework.h"

File::File(char *path, char file_mode) {
    if((file_mode == mode_read) && ((stat(path, &statbuf)) != 0)) {
        printf("Unable to get info about file %s.\n", path);
        throw "Unable to get info about file.";
    }
    if(file_mode == mode_write) {
        file = fopen(path, "wb");
        mode = mode_write;
    }
    if(file_mode == mode_append) {
        file = fopen(path, "ab");
        mode = mode_append;
    }
    if(file_mode == mode_read) {
        file = fopen(path, "rb");
        mode = mode_read;
    }
    if(file == NULL) {
        printf("Unable to open or create file %s.\n", path);
        throw "Unable to open or create file.";
    }
    else {
        fnsplit(path, drive, dir, name, ext);
        mask = 0x80;
        later = 0;
    }
    if_eof = false;
}

File::~File() {
    if(file != NULL) {
        if(mode == mode_write) {
            while(mask != 0x80) {
                PutBit(0);
            }
        }
        fclose(file);
    }
}

ulong File::GetBit() {
    if(mode != mode_read) {
    	printf("Unable to read bit.\n");
    	throw "Unable to read bit.";
    }
    if(mask == 0x80) {
    	later = fgetc(file);
    }
    if(later == EOF) {
        if_eof = true;
        return 0;
    }
    int temp;
    temp = mask & later;
    mask = mask >> 1;
    if(mask == 0) {
    	mask = 0x80;
    }
    if(temp > 0) return 1;
    return 0;
}

void File::PutBit(ulong bit) {
    if(mode != mode_write && mode != mode_append) {
    	printf("Unable to write bit.\n", name);
    	throw "Unable to write bit.";
    }
    if(bit != 0) later = later | mask;
    mask = mask >> 1;
    if(mask == 0x00) {
    	fputc(later, file);
    	mask = 0x80;
    	later = 0;
    }
    return;
}

ulong File::GetBits(int n) {
    ulong temp = 0;
    for(int i = 0;i < n;i++) {
	    temp = temp << 1;
	    temp = temp | GetBit();
    }
    return temp;
}

void File::PutBits(ulong bits, int n) {
    int i;
    ulong temp = 1;
    temp = temp << (n - 1);
    for(i = 0;i < n;i++) {
        if((temp & bits) > 0) {
            PutBit(1);
        }
	else {
	    PutBit(0);
	}
	temp = temp >> 1;
    }
    return;
}

int File::GetChar() {
    if(if_eof == true) return EOF;
    int i = (int)(GetBits(8));
    if((if_eof == true) && (mask == 0x80)) return EOF;
    return i;
}

void File::PutChar(int symb) {
    PutBits((ulong)(symb), 8);
}

void File::Write(char *a, int n) {
    for(int i = 0;i < n;i++) {
    	PutBits((ulong)(*a), 8);
    	a++;
    }
}

void File::Read(char *a, int n) {
    for(int i = 0;i < n;i++) {
    	*a = (char)(GetBits(8));
    	a++;
    }
}

void File::Rewind() {
    if(mode != mode_read) {
        printf("Unable to rewind file %s.", name);
        throw "Unable to rewind file.";
    }
    rewind(file);
    if_eof = false;
    mask = 0x80;
    later = 0;
    if_eof = 0;
}

File::File() {
    file = NULL;
    mode = mode_not_open;
}

void File::Open(char *path, char file_mode) {
    if(file != NULL) {
        if(mode == mode_write) {
            while(mask != 0x80) {
                PutBit(0);
            }
        }
        fclose(file);
    }
    if((file_mode == mode_read) && ((stat(path, &statbuf)) != 0)) {
        printf("Unable to get info about file %s.\n", path);
        throw "Unable to get info about file.";
    }
    if(file_mode == mode_write) {
        file = fopen(path, "ab");
        mode = mode_append;
    }
    if(file_mode == mode_write) {
        file = fopen(path, "wb");
        mode = mode_write;
    }
    if(file_mode == mode_read) {
        file = fopen(path, "rb");
        mode = mode_read;
    }
    if(file == NULL) {
        printf("Unable to open or create file %s.\n", path);
        throw "Unable to open or create file.";
    }
    else {
        fnsplit(path, drive, dir, name, ext);
        mask = 0x80;
        later = 0;
    }
    if_eof = false;
}

void File::Seek(long offset) {
    if(mode != mode_read) {
        printf("Unable to move position of reading in file.\n");
        throw "unable to move position";
    }
    fseek(file, offset, SEEK_CUR);
}

void File::NewLater() {
    if(mask != 0x80) {
        mask = 0x80;
        later = 0;
    }
}
