#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Table.h"
#include "TxtFunc.h"
#include "ext.h"
/**
 * Authors: Roni chen
 * Program Description: The program assembler gets from the user files' names without a suffix in the command line that contains assembly code.
 *  The assembler runs on each file and checks the syntax. 
 *  Afterwards it translates each line from the given files to Hexadecimal code which is the Machine code of each line.
 *  When the file is translated successfully the assembler creates 3 types of out files: 
 *   * <input file name>.ob = file that contains all the addresses and the Machine code value of each line from the given file.
 *   * <input file name>.ent = file that contains all the entry symbols in the given input file and their addresses.
 *   * <input file name>.ext = file that contains all the external symbols in the given input file and their addresses.
 *  The assembler uses 2 passes:
 *   * In the first pass, the assembler creates a symbol table and a structure of data and code memory picture and maintains it during the run.
 *   * In the second pass, the assembler fills the missing values to complete the memory picture, and then creates output files.
 * 
 */







table function;
list *codelist;
list *datalist;
symTbl *symbol;
extLst *ext;


void secondReadFile(FILE *file) {
    char lin[MAX_LINE];
    int coun = START_PLACE;
    lin[0] = '\0';

    while (fgets(lin, MAX_LINE, file) != NULL) {
        if (lin[strlen(lin) - 1] == '\n') {
            lin[strlen(lin) - 1] = '\0';
        }
        secondTrant(lin, &coun);
    }
}

void createAllLis() {
    symbol = createSymTab();
    codelist = createList();
    datalist = createList();
    ext = createExt();
}


void printTbl() {

    printf("Print:\n");
    printList(codelist);
    printList(datalist);
}


void writeOrDeleteTbl(const char *fl) {
    FILE *out;
    char *outName;

    outName = ((char *) malloc(sizeof(char) * (strlen(fl) + 3)));
    if (outName == NULL) {
        fprintf(stderr, "Not enough memory.\n");
        exit(NO_MEMMORY);
    }
    strcpy(outName, fl);
    strcat(outName, ".ob");

    out = fopen(outName, "w");
    if (out) {
        fprintf(out, "%X\t %X\n\n", codelist->coun - START_PLACE, datalist->coun - START_PLACE);
        writeFl(codelist, out);
        writeFl(datalist, out);
        fclose(out);
    } else {
        fprintf(stderr, "Error while tring to open %s file!\n", fl);
    }
    /*outName = strtok(outName, ".");*/
    writeExt(ext, fl);
    writeSymTab(symbol, fl);
    deleteLis(datalist);
    deleteLis(codelist);
}

void readFail(FILE *readfl) {
    int i = 0;
    char line[MAX_LINE] = "";
    line[i] = '\0';
    if (function == NULL) {
        function = createTableFunc();
    }

    while (fgets(line, MAX_LINE, readfl) != NULL) {
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        firstTrans(line);
    }
}


void openFile(char *namefl) {
    char name[70];
    FILE *flread;
    strcpy(name, namefl);

    strcat(name, ".as");  /* add the suffix to the file */
    printf("%s", name);

    flread = fopen(name, "r");

    if (flread) { /* open the current file with the given style and sets the pointer */

        createAllLis();

        readFail(flread);

        /* Set the data list to continue the values after the end of code list
            and update the symbol table in the changes */
        dataLocation(datalist, codelist->coun);
        addresses(symbol, codelist->coun);
        printTbl();
        freopen(name, "r", flread);
        secondReadFile(flread);
        fclose(flread);
        writeOrDeleteTbl(namefl);
    } else
        fprintf(stderr, "Error have a problem opening the file.%s\n", name);
}

/**
 * Function: main
 * Function Description: loop the given files and check the input inserted.
 * @param argc arguments count
 * @param argv arguments values
 * @return program exit 
 */

int main(int argc, char *argv[]) {
    int i;
    if (argc == 1 || argc == '\0') {
        fprintf(stderr, "No files to translate.\n");
        return NOT_EXIST;
    }

    for (i = 1; i < argc; i++) {
        openFile(argv[i]);
    }
    if (function)
        deleteTable(function);
    return 0;
}
