
#include "Table.h"


cell *newCell(short by, int place);


list *createList() {
    list *lst = NULL;
    if ((lst = (list *) malloc(sizeof(list))) == NULL) {
        fprintf(stderr, "Not enough memory.\n");
        exit(NO_MEMMORY);
    }

    lst->coun = START_PLACE;
    lst->first = lst->last = NULL;

    return lst;
}

void addList(list *lst, short by, int place)/* TODO: consider cat the place and uss the coun myself */
{
    /* create new cell */
    cell *ptr = newCell(by, place);

    if (lst->coun > START_PLACE)
        lst->last->next = ptr;
    else
        lst->first = ptr;

    /* Anyway, update the last and raise the coun */
    lst->last = ptr;
    lst->coun++;
}


cell *newCell(short by, int place) {
    cell *newCell;
    /* Allocate the new node. */
    if ((newCell = (cell *) malloc(sizeof(cell))) == NULL) {
        fprintf(stderr, "Not enough memory. Sorry.\n");
        exit(NO_MEMMORY);
    }

    newCell->by = by;
    newCell->place = place;
    newCell->next = NULL;

    return newCell;
}


void dataLocation(list *lst, int size) {
    cell *ptr;
    if (lst != NULL) {
        for (ptr = lst->first; ptr; ptr = ptr->next) {
            /* Add the size of other list to make this list follow it */
            ptr->place += size - START_PLACE;
        }
    }
}


cell *getCallList(list *lst, int place) {
    cell *ptr;
    if (lst != NULL) {
        for (ptr = lst->first; ptr; ptr = ptr->next) {
            if (ptr->place == place)
                return ptr;
        }
    }
    return NULL;
}


void printList(list *lst) {
    cell *ptr;
    if (lst != NULL) {
        for (ptr = lst->first; ptr; ptr = ptr->next) {
            printf(" %s\t %d\n", getString(ptr->by), ptr->place);
        }
    }
}


void writeFl(list *lst, FILE *outputFile) {
    cell *ptr;

    if (lst != NULL) {
        for (ptr = lst->first; ptr; ptr = ptr->next) {
            fprintf(outputFile, "%s\t %x\n",getString( ptr->place), ptr->by);
        }
    }
}


void deleteLis(list *lst) {
    cell *ptr;
    if (lst != NULL) {
        /* Use the first to save me the last cell before deleting */
        for (ptr = lst->first; ptr;) {
            lst->first = ptr->next;
            free(ptr);
            ptr = lst->first;
        }

        free(lst);
    }
}
