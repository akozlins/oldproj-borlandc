#ifndef __Filework_h
#define __Filework_h

#include <stdio.h>
#include <dir.h>
#include <stdlib.h>
#include <sys\stat.h>

#define mode_write 0x01
#define mode_read 0x02
#define mode_append 0x03
#define mode_not_open 0x04


typedef unsigned long ulong;

class File {
public:
    char drive[MAXDRIVE];
    char dir[MAXDIR];
    char name[MAXFILE];
    char ext[MAXEXT];
    bool if_eof;
    struct stat statbuf;
private:
    FILE *file;
    int later;
    int mask;
    char mode;
public:
    File();
    File(char *, char);
    void Open(char *, char);
    ulong GetBit();
    ulong GetBits(int);
    void PutBit(ulong);
    void PutBits(ulong, int);
    int GetChar(void);
    void PutChar(int);
    void Write(char *, int);
    void Read(char *, int);
    void Rewind();
    void Seek(long);
    ~File();
    void NewLater();
};

#endif