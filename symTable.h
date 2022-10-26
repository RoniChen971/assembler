
#include <ctype.h>
#include "Constants.h"


#define NUM_OF_LETTERS 52
#define ERROR_NAME_LABEL -200
#define NO_ADDRESS -66
#define ALREADY_EXIST -11


struct cell {
    type pt;
    int address;
    char name[MAX_LABEL + 1];
    struct cell *next;
};
typedef struct cell label;

struct hsTable {
    label *arr[NUM_OF_LETTERS];
};
typedef struct hsTable symTbl;


symTbl *createSymTab();

label *getLabel(symTbl *ht, const char name[MAX_LABEL + 1]);

void addresses(symTbl *ht, int size);

void writeSymTab(symTbl *ht, const char *flName);

void deleteSymTabl(symTbl *ht);

void printSymTab(symTbl *ht);

int addSymTab(symTbl *symbol, const char name[MAX_LABEL + 1], int adrs, type tp);






