#ifndef __Huff_h
#define __Huff_h

#define _error 0
#define _extract 1
#define _add 2
#define _remove 3
#define _view 4
#define _help 5

#define ar_id 0x123456
#define ar_id_l 0x12          
#define ar_id_m 0x34
#define ar_id_h 0x56

int Check(char **);
void Help();
void Add(char *, char *);
void Decode(char *, char *);
void List(char *);
void Delete(char *, char *);

class Huffman {
private:
public:
private:
public:
};

#endif