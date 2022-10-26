#include "ext.h"


extLst *createExt() {
    extLst *ext = NULL;

    if ((ext = (extLst *) malloc(sizeof(extLst))) == NULL) {
        fprintf(stderr, "Not enough memory.\n");
        exit(NO_MEMMORY);
    }

    ext->coun = 0;
    ext->first = ext->last = NULL;

    return ext;
}


label *newExt(int address, const char name[MAX_LABEL + 1]) {
    label *newlabel;
    /* Allocate the new node. */
    if ((newlabel = (label *) malloc(sizeof(label))) == NULL) {
        fprintf(stderr, "Not enough memory. Sorry.\n");
        exit(NO_MEMMORY);
    }

    newlabel->address = address;
    newlabel->pt = External;
    strcpy(newlabel->name, name);
    newlabel->next = NULL;

    return newlabel;
}


int addExt(extLst *ext, int address, char name[MAX_LABEL + 1]) {
    label *ptr;
    if (!isupper(name[0]) && !islower(name[0])) {
        fprintf(stderr, "%c (first char at \"%s\") is not a letter\n", name[0], name);

        return ERROR_NAME_LABEL;
    }

    ptr = newExt(address, name);

    if (ext->coun > 0)
        ext->last->next = ptr;
    else
        ext->first = ptr;

    ext->last = ptr;
    ext->coun++;
}


void deleteExt(extLst *lst) {
    label *ptr;
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


label *getExt(extLst *lst, const char name[MAX_LABEL + 1]) {
    /* Represents the pointer that we will run with */
    label *pTemp = lst->first;

    while (pTemp != NULL) {
        /* if the value already exist in this file */
        if (!strcmp(pTemp->name, name)) {
            return pTemp;
        }
        /* muve the pointer 1 label inside */
        pTemp = pTemp->next;
    }
    /* if the value is new in this file or not follow the rools */
    return NULL;
}


void writeExt(extLst *lst, const char *flName) {
    FILE *extFile;
    label *ptr;

    if (lst != NULL && lst->coun > 0) {
        /* Print all entrys */
        strcat(flName, ".ext");
        flName = fopen(&extFile, "w");
        if (flName == 0) {
            for (ptr = lst->first; ptr; ptr = ptr->next) {
                fprintf(extFile, "%s\t %b\n", ptr->name, ptr->address);
            }

            fclose(extFile);
        } else
            fprintf(stderr, "Error while tring to create %s file!\n", flName);

    }

    deleteExt(lst);
}
