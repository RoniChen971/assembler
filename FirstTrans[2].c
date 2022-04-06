#include <string.h>
#include <ctype.h>
#include "Constants.h"
#include "Table.h"
#include "TxtFunc.h"
#include "ext.h"

#define LAST_APOSTR -108
#define FIRST_APOSTR -109

extern table function;
extern list *codelist;
extern list *datalist;
extern symTbl *symbol;
extern extLst *ext;

#define getFirstNonSpaceIndex(i, str) for(; (i) < MAX_LINE&& isspace(str[(i)]); (i)++);


char *isLabel(char lbl[MAX_LINE]) {
    char lb[MAX_LINE];
    char *tokPoint;
    char *val;
    int lenOld = (int) strlen(lbl);

    strcpy(lb, lbl);
    tokPoint = strtok(lb, ":");
    if (tokPoint && strlen(tokPoint) != lenOld) {
        /* Allocate memory for the result becouse i need it out of this func */
        if ((val = (char *) malloc(sizeof(char) * (strlen(tokPoint) + 1))) == NULL) {
            puts("Not enough memory. Sorry.\n");
            exit(NO_MEMMORY);
        }
        strcpy(val, tokPoint);

        return val;
    } else
        return NULL;
}

int getFirstNonSpaceChar(char *str, int max) {
    int i;
    /* Run until real value */
    for (i = 0; i < max && isspace(str[i]); i++);
    return i;
}


char *isDataOrString(char ln[MAX_LINE], int *sizelb) {
    char *value;
    char *comma;
    int i;
    int j = 0;
    i = getFirstNonSpaceChar(&ln[*sizelb], MAX_LINE) + *sizelb;
    *sizelb = i;

    if (ln[i] == '.') {
        comma = strtok(&ln[i], ",");
        /* Allocate memory for the result becouse i need it out of this func */
        if ((value = (char *) malloc(sizeof(char) * (strlen(comma) + 1))) == NULL) {
            fprintf(stderr, "Not enough memory. Sorry.\n");
            exit(NO_MEMMORY);
        }
        strcpy(value, comma);

        return value;
    }

    return NULL;
}

int realNum(const char *str) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] != '+' && str[i] != '-' && !isspace(str[i]) && !isdigit(str[i]))
            return 0;
        i++;
    }
    return 1;
}

int checkInt(const char *str, int *out) {
    int mark;
    mark = (int) atoi(str);

    if (mark != 0 || realNum(str)) {
        *out = mark;
        return 0;
    } else {
        fprintf(stderr, "Error! %ls must be a real number\n", out);
        return DATA_ERROR;
    }
}

void addString(char ln[MAX_LINE], list *datalist) {
    int i;
    for (i = 0; ln[i] != FIRST_APOSTR; i++);

    for (i++; ln[i] != LAST_APOSTR; i++) {
        addList(datalist, ln[i], datalist->coun);
    }
    addList(datalist, '\0', datalist->coun);
}

void addData(char dat[MAX_LINE], list *datalist) {
    char *poin;
    int value;
    int i = getFirstNonSpaceChar(dat, MAX_LINE);

    poin = strtok(dat, ",");

    while (poin != NULL) {
        /* Try to get float out of float string if the string format is OK.
        otherwise return error code */
        if (checkInt(poin, &value) != DATA_ERROR)
            addList(datalist, garbageBit(value), datalist->coun);

        poin = strtok(NULL, ",");

    }
}


void careDataOrStr(const char lb[MAX_LINE], const char *dataOrStr, const char *lbl, int i) {
    char ln[MAX_LINE];
    int pla = i;
    pla += strlen(dataOrStr) + 1;

    strcpy(ln, lb);

    if (ln[0] == '.') {
        if (!strcmp(ln, ".string")) {
            if (lbl)
                addSymTab(symbol, lbl, datalist->coun * -1, Absolute);
            addString(&ln[pla], datalist);
        } else if (!strcmp(dataOrStr, ".data")) {
            if (lbl)
                addSymTab(symbol, lbl, datalist->coun * -1, Absolute);

            addData(&ln[pla], datalist);
        } else if (!strcmp(dataOrStr, ".extern")) {
            for (; pla < MAX_LINE && isspace(ln[pla]); pla++);
            addExt(ext, NO_ADDRESS, &lb[pla]);
        } else if (!strcmp(dataOrStr, ".entry"))
            fprintf(stderr, "Error: Unknown Command -> %s\n", dataOrStr);
    }
}


int isRegist(const char lb[OPERANDS_SIZE + 1]) {
    return lb[0] == 'r' && atoi(&lb[1]) >= 0 && atoi(&lb[1]) <= 7;
}


int isSame(const char *str, char c) {
    int i;
    for (i = 0; i < strlen(str); i++) {
        if (str[i] == c)
            return i;
    }
    return NOT_EXIST;
}


short operandID(const char prnd[OPERANDS_SIZE], typeOper *ope, int isFirstReg) {
    int i;
    for (i = 0; i < MAX_LINE && isspace(prnd[i]); i++);
    if (prnd[i] == '%') {
        *ope = distance;
        return createGarbaVa();
    } else if (prnd[i] == '#') {
        *ope = prompt;
        return createNumber(atoi(&prnd[1]), Absolute);
    } else if (isRegist(&prnd[i])) {
        *ope = regist;
        if (isFirstReg)
            return createRegist(Absolute, atoi(&prnd[i + 1]), 0);
        else
            return createRegist(Absolute, 0, atoi(&prnd[i + 1]));
    }
    *ope = direct;
    return createGarbaVa;
}

void careComm(const char lin[MAX_LINE], const char *dataOrStr, const char *lbl, int i) {
    char ln[MAX_LINE];
    int index = i;
    short opc = 0;
    char *poin;
    char nameOp[SIZE_NAME];
    char oper[OPERANDS_SIZE];
    short func;
    operandSum numOpe = operand_0;
    typeOper firstOper = prompt;
    typeOper secondOper = prompt;
    short headOper;
    short tailOper;

    if (lbl)
        addSymTab(symbol, lbl, codelist->coun, Absolute);

    strcpy(ln, lin);
    poin = strtok(&ln[index], " ");
    strcpy(nameOp, poin);
    opc = getOpcode(function, nameOp);
    if (opc == OPCODE_NOT_EXIST)
        fprintf(stderr, "Error: Command not recognized -> %s\n", dataOrStr);
    else {
        index += strlen(nameOp);
        for (; index < MAX_LINE && isspace(lin[index]); index++);
        numOpe = getNumOperand(function, opc);
        if (numOpe == operand_2) {
            poin = strtok(NULL, ",");
            if (!poin)
                fprintf(stderr, "Error: too little operands for %s\n", nameOp);

            else {

                strcpy(oper, poin);
                /*strcat(oper, ",");
                strcat(oper, strtok(NULL, ","));*/
            }
            headOper = operandID(oper, &firstOper, 1);
            /*poin += 1;  strtok(NULL, ",");*/
            index += strlen(poin) + 1;
            /*if(!strlen(lin + index)){

            }
            strcpy(ln, lin + index);*/
            for (; index < MAX_LINE && isspace(lin[index]); index++);

            if (!strlen(lin + index)) {
                fprintf(stderr, "Error: too little operands for %s\n", nameOp);
            } else {
                strcpy(oper, lin + index);
                tailOper = operandID(oper, &secondOper, 0);
            }
        }
    }

    if (numOpe == operand_1) {

        strcpy(ln, lin);
        strcpy(oper, &ln[index]);
        firstOper = operandID(oper, &secondOper, 1);
        addList(codelist, createWord(Absolute, secondOper, firstOper, opc, numOpe), codelist->coun);

        if (firstOper == regist && secondOper == regist)
            addList(codelist, createRegist(Absolute, headOper >> (REGISTER_SIZE + ENUMS_SIZE), tailOper >> ENUMS_SIZE),
                    codelist->coun);
        else {
            if (numOpe > operand_0)
                addList(codelist, tailOper, codelist->coun);
            if (numOpe == operand_2)
                addList(codelist, tailOper, codelist->coun);
        }
    }
}

void withoutVal(char *lbl) {
    if (isRegist(lbl))
        fprintf(stderr, "Error: Label name %s is non valid! register name!\n", lbl);
    if (getLabel(symbol, lbl) != NULL)
        fprintf(stderr, "Error: Label name %s already exist\n", lbl);
}


void firstTrans(const char lin[MAX_LINE]) {
    char *lbl;
    char *dataOrStr;
    int i = 0;
    char ln[MAX_LINE] = "";

    if (lin[0] == ';')
        return;

    for (; i < MAX_LINE && isspace(lin[i]); i++);
    if (!strlen(lin + i)) {
        return; /* continue for empty row */
    }
    strcpy(ln, lin + i);
    lbl = isLabel(ln);
    if (lbl) {
        withoutVal(lbl);
        dataOrStr = strtok(ln, ":");
        i = strlen(lbl) + 1;
    }
    dataOrStr = isDataOrString(lin, &i);
    strcpy(ln, lin);

    if (dataOrStr)
        careDataOrStr(lin, dataOrStr, lbl, i);

    else
        careComm(lin, dataOrStr, lbl, i);

    i = 0;

    if (dataOrStr)
        free(dataOrStr);

    if (lbl)
        free(lbl);
}





