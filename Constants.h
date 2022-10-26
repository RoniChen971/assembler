#ifndef Constants_h
#define Constants_h

/* Sizes byte */
#define OPCODE_SIZE 4
#define FUNC_SIZE 4
#define OPERANDS_SIZE (MAX_LINE/4)
#define MAX_LABEL 31
#define MAX_LINE 82
#define WORD_SIZE 12
#define REGISTER_SIZE 12
#define MAX_OPERAND 2
#define NO_MEMMORY -5
#define NOT_EXIST -2999
#define ENUMS_SIZE 2

#define START_PLACE 100


/*Errors*/
#define NO_BIT -6
#define ERROR -9999
#define DATA_ERROR -1001


/* enums  */
typedef enum {
    prompt,
    direct,
    distance,
    regist
} typeOper;


typedef enum {
    Absolute,
    External,
    Relocatable
} type;

typedef enum {
    operand_0,
    operand_1,
    operand_2
} operandSum;


int checkInt(const char *str, int *out);

void firstTrans(const char lin[MAX_LINE]);

int isRegist(const char lb[OPERANDS_SIZE + 1]);

int isSame(const char *str, char c);

char *isLabel(char lbl[MAX_LINE]);

void secondTrant(const char line[MAX_LINE], int *coun);

short garbageBit(short by);

short createGarbaVa();

short createNumber(short num, type ERA);

char *getString(int num);

short createWord(type ERA, typeOper destination, typeOper source, short opc, operandSum group);

short createRegist(type ERA, short regiSurc, short regiDest);

char *isDataOrString(char ln[MAX_LINE], int *sizelb);


#endif
