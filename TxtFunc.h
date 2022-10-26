

#include "Constants.h"

#define SIZE_NAME 5
#define COMMANDS 16
#define OPCODE_NOT_EXIST 17
#define ERROR_NAME -60
#define    table opcod*


struct txtFunc {
    char name[SIZE_NAME];
    unsigned short opc;
    unsigned short func;
    operandSum oper;
};
typedef struct txtFunc opcod;

table createTableFunc();

short getOpcode(table func, const char *name);

short getFunc(table funct, const char *name);

operandSum getNumOperand(table func, short opcode);

void deleteTable(table func);


