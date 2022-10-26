#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symTable.h"

label *newlabel(const char flName[MAX_LABEL + 1], int adrs, type pt);

symTbl *createSymTab() {
    symTbl *symbol = NULL;
    int i;
    if ((symbol = (symTbl *) malloc(sizeof(symTbl))) == NULL) {
        fprintf(stderr, "Not enough memory. Sorry.\n");
        exit(NO_MEMMORY);
    }
    for (i = 0; i < NUM_OF_LETTERS; i++)
        symbol->arr[i] = NULL;

    return symbol;
}

int hush(const char name[MAX_LABEL + 1]) {
    /*return value*/
    int rv = 0;
    /* The key is the first letter that has to be 1 of the NUM_OF_LETTERS letters*/
    char key = name[0];

    /* If key is a capitol letter its place is in the second half of array */
    if (isupper(key))
        rv = (key - 'A');
    else if (islower(key))
        rv = (key - 'a');
    else {
        rv = ERROR_NAME_LABEL;
        fprintf(stderr, "%c (first char at \"%s\") is not a letter\n", key, name);
    }
    return rv;
}

label *getLabel(symTbl *ht, const char name[MAX_LABEL + 1]) {
    int key = hush(name);
    /* Represents the pointer that we will run with */
    label **pTemp;

    if (key != ERROR_NAME_LABEL) {
        pTemp = &(ht->arr[key]);

        while (*pTemp != NULL) {
            /* if the value already exist in this file */
            if (!strcmp((*pTemp)->name, name)) {
                /*(*pTemp)->address++;*/
                return *pTemp;
            }
            /* muve the pointer 1 label inside */
            pTemp = &((*pTemp)->next);
        }
    }
    /* if the value is new in this file or not follow the rools */
    return NULL;
}

int addSymTab(symTbl *symbol, const char name[MAX_LABEL + 1], int adrs, type tp) {
    int key = hush(name);
    /* Represents the pointer that we will run with */
    label **pTemp;

    /* if the value does not follow the Labels rools */
    if (key == ERROR_NAME_LABEL)
        return ERROR_NAME_LABEL;
        /* if the value already exist in this file */
    else if (getLabel(symbol, name) != NULL)
        return ALREADY_EXIST;
        /* If the value is new */
    else {
        pTemp = &(symbol->arr[key]);

        while (*pTemp != NULL) {
            /* move the pointer 1 label inside */
            pTemp = &((*pTemp)->next);
        }

        /* Create new label for it */
        *pTemp = newlabel(name, adrs, tp);

        return 0;
    }
}

label *newlabel(const char name[MAX_LABEL], int adrs, type tp) {
    label *newlabel;
    if ((newlabel = (label *) malloc(sizeof(label))) == NULL) {
        fprintf(stderr, "Not enough memory. Sorry.\n");
        exit(NO_MEMMORY);
    }

    /* init the new label with the relevant values and return it */
    strcpy(newlabel->name, name);
    newlabel->address = adrs;
    newlabel->pt = tp;
    newlabel->next = NULL;

    return newlabel;
}

void deleteSymTabl(symTbl *ht) {
    int i;
    label *pTemp;

    for (i = 0; i < NUM_OF_LETTERS; i++) {
        if (ht->arr[i] != NULL) {
            /* Use the pointer itself to save me the last label before deleting */
            pTemp = ht->arr[i];
            while (pTemp) /* while(pTemp != NULL)*/
            {
                ht->arr[i] = pTemp->next;
                free(pTemp);
                pTemp = ht->arr[i];
            }
        }
    }
    free(ht);
}

void writeSymTab(symTbl *ht, const char *flName) {
    FILE *entFile;
    label *ptr;
    char *val;
    int i;

    if (ht != NULL) {
        val = (char *) malloc(sizeof(char) * (strlen(flName) + 5));
        if (val == NULL) {
            fprintf(stderr, "Not enough memory.\n");
            exit(NO_MEMMORY);
        }
        /* Add ent finish */
        strcpy(val, flName);
        strcat(val, ".ent");
        entFile = fopen(val, "w");
        if (entFile) {
            for (i = 0; i < NUM_OF_LETTERS; i++) {
                ptr = ht->arr[i];
                while (ptr != NULL) {
                    /* Print all entrys */
                    if (ptr->pt == External)
                        fprintf(entFile, "%s\t %X\n", ptr->name, ptr->address);
                    ptr = ptr->next;
                }
            }
            fclose(entFile);
        } else
            fprintf(stderr, "Error while tring to create %s file!\n", flName);

        /* Remove ent finish */
        free(val);
    }
    deleteSymTabl(ht);
}

void printSymTab(symTbl *ht) {
    int i;
    label *trans;
    for (i = 0; i < NUM_OF_LETTERS; i++) {
        if (ht->arr[i] != NULL) {
            printf("%d -", i);
            trans = ht->arr[i];
            while (trans != NULL) {
                printf(" %s %d ", trans->name, trans->address);

                /* put ',' only if there is another one after */
                if (trans->next != NULL)
                    printf(",");

                trans = trans->next;
            }
            printf("\n");
        }
    }
}

/**
 * Function: addFix
 * Function Description: inserts the given address and symbol to fix list
 * @param address the address to save
 * @param sym the symbol name
 * @param fd file information
 */
void addresses(symTbl *ht, int size) {
    int i;
    label *ptr;

    for (i = 0; i < NUM_OF_LETTERS; i++) {
        if (ht->arr[i] != NULL) {
            /* Use the pointer itself to save me the last label before deleting */
            ptr = ht->arr[i];
            while (ptr)/*while(pTemp != NULL)*/
            {
                /* Find the negative addresses (the ones from .string and .data)
                and update them to positive number after the code ones */
                if (ptr->address < 0)
                    ptr->address = ptr->address * -1 + size - START_PLACE;
                ptr = ptr->next;
            }
        }
    }
}

