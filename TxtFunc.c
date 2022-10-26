#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "TxtFunc.h"


void addDatafunc(table tbl, char name[SIZE_NAME], short index, unsigned short opco, unsigned short functi,
                 operandSum operan) {
    strcpy(tbl[index].name, name);
    tbl[index].opc = opco;
    tbl[index].func = functi;
    tbl[index].oper = operan;
}

table createTableFunc() {
    table temp;
    short i = 0;

    temp = (table*) malloc(COMMANDS * sizeof(struct txtFunc));

    if (temp == NULL) {
        fprintf(stderr, "Not enough memory.\n");
        exit(NO_MEMMORY);
    }

    /* Init all with values. */
    addDatafunc(temp, "mov", i++, 0, 0, operand_2);
    addDatafunc(temp, "cmp", i++, 1, 0, operand_2);
    addDatafunc(temp, "add", i++, 2, 10, operand_2);
    addDatafunc(temp, "sub", i++, 2, 11, operand_2);
    addDatafunc(temp, "lea", i++, 4, 0, operand_2);
    addDatafunc(temp, "clr", i++, 5, 10, operand_1);
    addDatafunc(temp, "not", i++, 5, 11, operand_1);
    addDatafunc(temp, "inc", i++, 5, 12, operand_1);
    addDatafunc(temp, "dec", i++, 5, 13, operand_1);
    addDatafunc(temp, "jmp", i++, 9, 10, operand_1);
    addDatafunc(temp, "bne", i++, 9, 11, operand_1);
    addDatafunc(temp, "jsr", i++, 9, 12, operand_1);
    addDatafunc(temp, "red", i++, 12, 0, operand_1);
    addDatafunc(temp, "prn", i++, 13, 0, operand_1);
    addDatafunc(temp, "rts", i++, 14, 0, operand_0);
    addDatafunc(temp, "stop", i, 15, 0, operand_0);

    return temp;
}


short getOpcode(table func, const char *name) {
    int i;

    for (i = 0; i < COMMANDS; i++) {
        if (!strcmp(func[i].name, name))
            return func[i].opc;
    }
    return OPCODE_NOT_EXIST;
}


short getFunc(table funct, const char *name) {
    int i;
    for (i = 0; i < COMMANDS; i++) {

        if (!strcmp(funct[i].name, name))
            return funct[i].func;
        i++;
    }
    return OPCODE_NOT_EXIST;
}

operandSum getNumOperand(table func, short opcode) {
    return func[opcode].oper;

}


void deleteTable(table func) {
    free(func);
}

