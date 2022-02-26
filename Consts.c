
#include <stdio.h>
#include <string.h>
#include "Consts.h"
#include "stdlib.h"
#include "help.h"


int DC=1;
int IC=100;

registers registList[]=
{
    {"r0", "000000000001", ""},
    {"r1", "000000000010", ""},
    {"r2", "000000000100", ""},
    {"r3", "000000001000", ""},
    {"r4", "000000010000", ""},
    {"r5", "000000100000", ""},
    {"r6", "000001000000", ""},
    {"r7", "000010000000", ""},
};

operation operations[]=
{
    {"mov","00000000",{0,1,3}, {1,3}},
    {"cmp","00010000",{0,1,3}, {0,1,3}},
    {"add","00101010",{0,1,3}, {1,3}},
    {"sub","00101011",{0,1,3}, {1,3}},
    {"lea","01000000",{1}, {1,3}},
    {"clr","01011010",{-1}, {1,3}},
    {"not","01011011",{-1}, {1,3}},
    {"inc","01011100",{-1}, {1,3}},
    {"dec","01011101",{-1}, {1,3}},
    {"jmp","10011010",{-1}, {1,2}},
    {"bne","10011011",{-1}, {1,2}},
    {"jsr","10011100",{-1}, {1,2}},
    {"red","11000000",{-1}, {1,3}},
    {"prn","11010000",{-1}, {0,1,3}},
    {"rts","11100000",{-1}, {-1}},
    {"stop","11110000",{-1}, {-1}},
};

symbol *SymbolsTable= NULL;
image *codeImage = NULL;
image *dataImage = NULL;
externs *externTable=NULL;

void getRgisterCode(int i ,char* code)/*return code for register i*/
{
    strcpy(code,registList[i].code);
           
}

void  getOeration(char* nameOp, operation* dist)
{
    
     int i;

    for(i =0; i<16; i++){
        if (!strcmp(operations[i].name,nameOp))
        {
            
            *dist=operations[i];
        }
    }
}

int addSymbol(char *name, int value, char att)/*insert new symbol or update new attribute in symbol exist*/
{
    symbol *p= SymbolsTable;
    symbol * newSymbol = (symbol *)malloc(sizeof(symbol));
    
    if(SymbolsTable == NULL)/*the head*/
    {
        SymbolsTable = (symbol *)malloc(sizeof(symbol));
        SymbolsTable->atrr[0] = att;
        if(att == 'x'){/*without ':' in the end*/
            SymbolsTable->name = malloc(strlen(name));
            strncpy(SymbolsTable->name, name,strlen(name));
        }
        else{
        SymbolsTable->name = malloc(strlen(name)-1);
        strncpy(SymbolsTable->name, name,strlen(name)-1);
        }
        SymbolsTable->next = NULL;
        SymbolsTable->value = value;
        return 1;
    }
    
    
    
    while(p->next != NULL)
    {
        if(!strcmp(p->name, name))/*update new attr*/
        {
            if ((p->atrr[0] == att)||(p->atrr[1] !='\0'))
                return  0;
            if((p->atrr[0] =='c' && att=='d')||(p->atrr[0] =='d' && att=='c'))
                return 0;
            if(p->atrr[0] =='n' && att=='x')
                  return 0;
            if(p->atrr[0] =='x' && (att=='c'|| att=='d'|| att=='n'))
                  return 0;
            
            p->atrr[1] =att;
            return  1;
        }
               
        p= p->next;
    }
    
    /*for new node*/
    newSymbol->atrr[0] = att;
    if(att == 'x'){/*without ':' in the end*/
        newSymbol->name = malloc(strlen(name));
        strncpy(newSymbol->name, name,strlen(name));
    }
    else{
    newSymbol->name = malloc(strlen(name)-1);
        strncpy(newSymbol->name, name,strlen(name)-1);
        
    }
    newSymbol->next = NULL;
    newSymbol->value = value;
    
    p->next = newSymbol;
    
    return  1;
}

int insertString(char *word)
{
    int i;
    
    for (i =0; i<=strlen(word); i++)/*for each char in the word*/
    {
        image *p= dataImage;
        image * newData = (image *)malloc(sizeof(image));
    if(dataImage == NULL)/*the head*/
    {
        dataImage = (image *)malloc(sizeof(image));
        dataImage->next = NULL;
        dataImage->adress = DC;
        dataImage->type = 'A';
        convBinry(dataImage->coding,word[i]);/*add acsii code in*/
        continue;
    }
       
       
    
        while(p->next != NULL)
        {
            p= p->next;
        }
        /*for new node*/
        newData->next = NULL;
        DC++;
        newData->adress = DC;
        newData->type = 'A';
        convBinry(newData->coding,word[i]);/*add acsii code in*/
        p->next = newData;
    }
    
    
    return  1;
}
void freeSymbol(void)
{
    symbol *p;
    symbol *q;
    
    for (p=SymbolsTable; p!=NULL; p=q) {
        q=p->next;
        free(p);
    }
    
}

void freeImage(void)
{
    image *p;
    image *q;
    
    for (p=codeImage; p!=NULL; p=q) {
        q=p->next;
        free(p);
    }
    
    
}
int insertData(int digit)
{
    image *p= dataImage;
    image * newData = (image *)malloc(sizeof(image));
    
    if(dataImage == NULL)/*for head*/
    {
        dataImage = (image *)malloc(sizeof(image));
        dataImage->next = NULL;
        dataImage->adress = DC;
        dataImage->type = 'A';
        convBinry(dataImage->coding,digit);/*add acsii code in*/
        return  1;
    }
       
   
    while(p->next != NULL)
    {
        p= p->next;
    }
   
    /*for new node*/
    newData->next = NULL;
    DC++;
    newData->adress = DC;
    newData->type = 'A';
    convBinry(newData->coding,digit);/*add acsii code in*/

    p->next = newData;
    return  1;
}

int insertInstruction(char * code)
{
    image *p= codeImage;
    image *newData = (image *)malloc(sizeof(image));
    
    if(codeImage == NULL)/*for head*/
    {
        codeImage = (image *)malloc(sizeof(image));
        codeImage->next = NULL;
        codeImage->adress = IC;
        if(code[0] == '?')
            codeImage->type = '?';
        else
            codeImage->type = 'A';
        strcpy(codeImage->coding, code);
        
        return  1;
    }
       
    

    while(p->next != NULL)
    {
        p= p->next;
    }
    /*for new node*/
    newData->next = NULL;
    newData->adress = IC;
    if(code[0] == '?')
        newData->type = '?';
    else
        newData->type = 'A';
    strcpy(newData->coding, code);

    p->next = newData;
    return  1;
}

void updateDc()/*update dc counter in dataImage after first skim*/
{
    image *p= dataImage;
    image *q= codeImage;
    
    if (p!=NULL)
    {
        while (p!=NULL)
        {
            p->adress+=IC;
            p=p->next;
        }
    }
    if (q!=NULL)
    {
        while (q->next!=NULL)
            q=q->next;
        q->next=dataImage;
    }
    
}
int checkSymbol(char *word)/*find if symbol is exist*/
{
    symbol *p = SymbolsTable;
    
    if (p!=NULL)
    {
        while (p!=NULL)
        {
            if (!strcmp(p->name, word))
                return 1;
            p=p->next;
        }
    }
    return 0;
}
int getValueSymbol(char *word)
{
    symbol *p = SymbolsTable;
    
    if (p!=NULL)
    {
        while (p!=NULL)
        {
            if (!strcmp(p->name, word))
                return p->value;
            p=p->next;
        }
    }
    return 0;
}
int updateCodeLable(int add, char *code,int value)/*update the type in the second skim*/
{
    image *p= codeImage;
    if (p!=NULL)
    {
        while (p!=NULL)
        {
            if (p->adress ==add)
            {
                strcpy(p->coding, code);
                if (!value)
                    p->type='E';
                else
                    p->type='R';
                return 1;
            }
            p=p->next;
        }
    }
    return 0;
}
image* getHeadCodeImage()
{
    image *p = codeImage;
    
    return p;
    
}
symbol* getHeadSymbolTable()
{
    symbol*p=SymbolsTable;
    return p;
}
int addExtern (char* name, int value)/*in the second skim find all extern */
{
    externs *p= externTable;
    externs *newExtern = (externs *)malloc(sizeof(externs));
    
    if(externTable == NULL)/*for head*/
    {
        externTable = (externs *)malloc(sizeof(externs));
        externTable->next = NULL;
        externTable->name = malloc(strlen(name));
        strncpy(externTable->name, name,strlen(name));
        externTable->address = value;
        return  1;
    }
    while(p->next != NULL)
    {
        p= p->next;
    }
    /*for new node*/
    newExtern->next = NULL;
    
    strcpy(newExtern->name, name);
    newExtern->address=value;
    p->next = newExtern;
    return 1;
}
externs* getHeadExternTable()
{
    externs *p=externTable;
    return p;
}
void updateSymbolValue()/*for change the value of dc after first skim*/
{
    symbol *p= SymbolsTable;
    
    if (p!=NULL)
    {
        while (p!=NULL)
        {
            if(p->atrr[0] == 'd'){
                p->value+=IC;
            }
            p=p->next;
        }
    }
}
