


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symTable.h"


struct list {
    label *first;
    label *last;
    int coun;
};
typedef struct list extLst;


extLst *createExt();

int addExt(extLst *ext, int address, char name[MAX_LABEL + 1]);

label *getExt(extLst *lst, const char name[MAX_LABEL + 1]);

void writeExt(extLst *lst, const char *flName);
