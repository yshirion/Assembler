

#ifndef Consts_h
#define Consts_h

#include <stdio.h>

typedef struct
{
    char *name;
    char* code;
    int opSourc[3];
    int opDes[3];
}operation;
operation operations[16];

typedef struct symbols
{
    char *name;
    int value;
    char atrr[2];
    struct symbols *next;
}symbol;


typedef struct images
{
    int adress;
    char coding [13];
    char type;
    struct images *next;
}image;


typedef struct
{
    char *name;
    char *code;
    char *value;
}registers;
registers registList[8];

typedef struct externStrct
{
    char* name;
    int address;
    struct externStrct *next;
    
}externs;


void  getRgisterCode(int i, char * code);
void  getOeration(char* nameOp, operation* dist);
int addSymbol(char *name, int value, char att);
int insertString(char *word);
int insertData(int digit);
int insertInstruction(char * code);
void updateDc(void);
void freeSymbol(void);
void freeImage(void);
int checkSymbol(char *word);
int getValueSymbol(char *word);
int updateCodeLable(int add, char * code, int value);
image* getHeadCodeImage(void);
symbol* getHeadSymbolTable(void);
int addExtern (char* name, int value);
externs* getHeadExternTable(void);
void updateSymbolValue(void);

#endif /* Consts_h */
