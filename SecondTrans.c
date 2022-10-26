#include <string.h>
#include <ctype.h>
#include "Constants.h"
#include "Table.h"
#include "ext.h"
#include "TxtFunc.h"

/* Macro to get the maximum value between 2 */
#define max2(x, y) ((x) > (y) ? (x) : (y))
/* Macro to get the maximum value between 3 */
#define max3(x, y, z) ((x) > (max2(z,y)) ? (x) : (max2(z,y)))
/* Macro to get the absulut value */
#define abs(x)  ((x) < 0 ? -(x) : (x))

extern table function;
extern list *codelist;
extern list *datalist;
extern symTbl *symbol;
extern extLst *ext;


void careDataOrStr2(const char line[MAX_LINE], const char *dataOdstr, int place) {
    label *tem;
    char ln[MAX_LINE];
    int i = place;
    i += strlen(dataOdstr) + 1;

    strcpy(ln, line);

    if (!strcmp(dataOdstr, ".entry")) {
        /* Find the sim in symbol table and update it to Entry */
        for (; i < MAX_LINE && isspace(ln[i]); i++);
        if (tem = getLabel(symbol, &line[i]))
            tem->pt = Relocatable;
        else
            fprintf(stderr, "Error: The entry Label %s is not defined\n", dataOdstr);
    }
}


short careDisAddress(const char opre[MAX_LINE], int coun) {
    char temp[MAX_LINE];
    char firstLbl[MAX_LINE];
    char secondLbl[MAX_LINE];
    char *mark;
    int i = 2;
    int firstAdd;
    int secondAdd;
    int result;

    for (; i < MAX_LINE && isspace(opre[i]); i++);

    strcpy(temp, &opre[i]);

    mark = strtok(temp, ",");
    strcpy(firstLbl, mark);
    mark = strtok(NULL, ",");
    strcpy(secondLbl, mark);

    firstAdd = getLabel(symbol, firstLbl)->address;
    secondAdd = getLabel(symbol, secondLbl)->address;

    result = max3(abs(firstAdd - secondAdd), abs(firstAdd - coun), abs(secondAdd - coun));
    return createNumber(result, Absolute);
}

short careDireAddress(const char opre[MAX_LINE], int OperSecond, int coun) {
    label *ptr;
    if (ptr = getLabel(symbol, opre))
        return createNumber(ptr->address, Relocatable);
    else {
        if (ptr = getExt(ext, opre)) {
            if (ptr->address == NO_ADDRESS)
                ptr->address = coun + 1 + OperSecond;
            else
                addExt(ext, coun + 1 + OperSecond, opre);
            return createNumber(0, External);
        }
    }

    return createGarbaVa();
}

short careOper2(const char opre[MAX_LINE], typeOper *oper, int OperSecond, int coun) {
    int i;
    for (i = 0; i < MAX_LINE && isspace(opre[i]); i++);
    if (opre[i] == '%') {
        *oper = distance;
        return createGarbaVa(&opre[i], coun);
    } else if (opre[i] == '#') {
        *oper = prompt;
        return createGarbaVa();
    } else if (isRegist(&opre[i])) {
        *oper = regist;
        return createGarbaVa();
    }
    *oper = direct;
    return careDireAddress(&opre[i], OperSecond, coun);
}

void careComma2(const char line[MAX_LINE], const char *daOrStr, const char *lbl, int place, int *coun) {
    char ln[MAX_LINE];
    char *mark;
    operandSum opcNum = operand_0;
    typeOper firstOperand = prompt;
    typeOper secondOperand = prompt;
    short firstOper;
    short secondOper;
    int i = place;
    short opcd = 0;
    char nameOp[SIZE_NAME];
    char oper[MAX_LINE];


    strcpy(ln, line);
    mark = strtok(&ln[i], ",");
    strcpy(nameOp, mark);

    opcd = getOpcode(function, nameOp);
    if (opcd != OPCODE_NOT_EXIST) {
        i += strlen(nameOp);
        for (; i < MAX_LINE && isspace(line[i]); i++);
        opcNum = getNumOperand(function, opcd);
        if (opcNum == operand_2) {
            mark = strtok(NULL, ",");
            if (mark) {
                strcpy(oper, mark);
                mark = strtok(NULL, ",");
                firstOper = careOper2(oper, &firstOperand, 0, *coun); /* First operand*/
                if (mark) {
                    strcpy(oper, mark);
                    secondOper = careOper2(oper, &secondOperand, 1, *coun); /* Second operand*/
                }
            }
        } else if (opcNum == operand_1) {
            strcpy(ln, line);
            strcpy(oper, &ln[i]);
            firstOper = careOper2(oper, &secondOperand, 0, *coun); /* Second operand */
        }

        (*coun)++; /* increase the coun for the command byte that must be */

        /* We only have to update table for distance & direct */
        if (firstOperand == distance || firstOperand == direct)
            getCallList(codelist, *coun)->by = firstOper;
        if (secondOperand == distance || secondOperand == direct) {
            /* For 1 operand we update the first one */
            if (opcNum == 1)
                getCallList(codelist, *coun)->by = firstOper;
            else
                getCallList(codelist, *coun + 1)->by = secondOper;
        }
        if (firstOperand == regist && secondOperand == regist)
            *coun += 1; /* the argument that contains them both */
        else
            *coun += opcNum; /* the arguments */
    }
}


void secondTrant(const char line[MAX_LINE], int *coun) {
    int i = 0;
    char *lbl;
    char *daOrStr;
    char ln[MAX_LINE];

    for (; i < MAX_LINE && isspace(line[i]); i++);
    if (line[0] == ';')
        return;

    if (!strlen(line + i)) {
        return; /* continue for empty row */
    }

    strcpy(ln, line + i);
    lbl = isLabel(ln);
    if (isLabel(ln))
        i = strlen(lbl) + 1;

    daOrStr = isDataOrString(ln, &i);
    strcpy(ln, line);

    if (daOrStr)
        careDataOrStr2(line, daOrStr, i);
    else
        careComma2(line, daOrStr, lbl, i, coun);

    i = 0;

    if (daOrStr)
        free(daOrStr);
    if (lbl)
        free(lbl);
}

