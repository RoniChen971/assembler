#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Constants.h"
#include "TxtFunc.h"

#define GARBAGE_VALUE -16
#define GARBAGE_ICON '?'


/* Return string of 0s or 1s represents by in length of 12
Uses only for debug prints */
char *getString(int num) {
    char *value = (char *) malloc(sizeof(char) * (WORD_SIZE + 1));
    int temp = num;
    int i;
    if (num == GARBAGE_VALUE) {
        for (i = WORD_SIZE - 1; i >= 0; i--) {
            value[i] = GARBAGE_ICON;

        }
    } else
        for (i = WORD_SIZE - 1; i >= 0; i--) {
            value[i] = (temp & 1) + '0';
            /* Push the bits 1 bits left and now we have a new bit in the leftest bit */
            temp >>= 1;
        }

    value[WORD_SIZE] = '\0';
    return value;
}


short createWord(type ERA, typeOper destination, typeOper source, short opc, operandSum group) {
    short rv = 0;

    /* Add the leftest value and push it left and so on */
    rv = (rv | group) << OPCODE_SIZE;
    rv = (rv | opc) << ENUMS_SIZE;
    rv = (rv | source) << ENUMS_SIZE;
    rv = (rv | destination) << ENUMS_SIZE;
    rv = rv | ERA;

    return rv;

}

short createNumber(short num, type ERA) {
    short rv = 0;

    /* Add the leftest value and push it left and so on */
    rv = (rv | num) << ENUMS_SIZE;
    rv = (rv | ERA);

    return garbageBit(rv);
}


short createRegist(type ERA, short registerNumSrc, short registerNumDst) {
    short rv = 0;

    /* Add the leftest value and push it left and so on */
    rv = (rv | registerNumSrc) << REGISTER_SIZE;
    rv = (rv | registerNumDst) << ENUMS_SIZE;
    rv = (rv | ERA);

    return rv;
}


short createGarbaVa() {
    return GARBAGE_VALUE;
}

short garbageBit(short by) {
    /* move out the non relevant bits */
    return by & 0xFFF;
}


