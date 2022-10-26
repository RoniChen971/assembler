#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Constants.h"


# define MAX_NAME_GUIDANCE 8


struct entry {
    short by;
    int place;
    struct entry *next;
};
typedef struct entry cell;

struct lst {
    cell *first;
    cell *last;
    int coun;
};
typedef struct lst list;


list *createList();

void addList(list *lst, short by, int place);

void dataLocation(list *lst, int size);

cell *getCallList(list *lst, int place);

void writeFl(list *lst, FILE *outputFile);

void deleteLis(list *lst);

void printList(list *lst);




