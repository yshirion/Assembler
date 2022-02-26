
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "FirstSkim.h"
#include "CheckError.h"
#include "Consts.h"
#include "help.h"

#define space while (line[place]==' '||line[place]=='\t') place++;
#define MAXLEN 80


extern int IC;
extern int DC;
int ICF;
int IDF;
int place=0;
static char line[81];
static char word [32];
int numOfLine;
int isLabel=0;
int isExtern=0;
operation *p;
char *reservedWord[]=
{
    ".data",".string",".extern",".entry","mov","cmp","add","sub","lea","clr","not","inc","dec","jmp","bne","jsr","red","prn","rts","stop","r0","r1","r2","r3","r4","r5","r6","r7"
};


int doFirstSkim (FILE *f)
{
    int res;
    int proper=1;
    const char endl = '\n';
    char *ret = NULL;
    char c;

    numOfLine = 0;
    while(fgets(line, sizeof(line), f) != NULL)
    {
        numOfLine ++;
        /*here we need to check line*/
        
        
        /*Here we check if the length of line is proper, and find '\n'.*/
        ret = strchr(line, endl);
        if (!ret)
        {
            printError(1, numOfLine);
            while ((c=getc(f))!=EOF)
            {
                if (c=='\n')
                    break;
            }
        }
        
        res = checkLine();
        if (!res) proper=0;
        memset(line,'\0',(int)sizeof(line));
    }
    
    if(!proper)/*if there is error if the file*/
        return proper;
    IC--;
    updateDc();
    updateSymbolValue();
    ICF=IC-100;
    IDF=DC;
    return proper;
}


int checkLine()
{
    int proper = 1;
    place = 0;
    isLabel=0;
    
    space
    if (line[place]==';'||line[place]=='\n') {
        return 1;
    }
    
    
    checkWord();
    proper= checkLabel();
    if (!proper) return 0;
    if (word[place-1]==':')
    {
        isLabel=1;
        proper=careLine();
        return proper;
    }
    space
    
    place = place-(int)strlen(word)-1;
    proper=careLine();
    /*if (!proper) return 0;*/
    return proper;
}
int careLine()
{
    char preWord[32];
    int proper=1;
    strcpy(preWord, word);

    checkWord();
    if(!(strcmp(word, ".data"))){
        proper=careData(preWord);
        return proper;
    }
       
    if(!(strcmp(word, ".string"))){
        proper=careString(preWord);
        return proper;
    }
    
    if (!(strcmp(word, ".entry"))) {
        if (isLabel)
            printError(7, numOfLine);
        return proper;
    }
    if (!(strcmp(word, ".extern"))) {
        isExtern=1;
        if (isLabel)
            printError(7, numOfLine);
        checkWord();
        proper= checkLabel();
        if (proper){
            addSymbol(word, 0, 'x');
        }
        return proper;
    }
    proper=careInstruction(preWord);
    return proper;
}
int checkLabel ()
{

    int j;
    char tmpWord[32];
    
    memset(tmpWord, '\0', strlen(tmpWord));

    if (word[place-1]==':')
    {
        if (strlen(word)>32)
        {
            printError(2, numOfLine);
            return 0;
        }
        else
        {
            strncpy(tmpWord,word,strlen(word)-1);
            for (j=0;j<28;j++)/*for 28 savedwords*/
            {
                if (tmpWord==reservedWord[j])
                {
                    printError(3, numOfLine);
                    return 0;
                }
            }
        }return 1;
    }
    else
    {
        if (isExtern==1)
        {
            if (strlen(word)>31)
            {
                printError(2, numOfLine);
                return 0;
            }
            else
            {
                strncpy(tmpWord,word,strlen(word));
                for (j=0;j<28;j++)/*for 28 savedwords*/
                {
                    if (tmpWord==reservedWord[j])
                    {
                        printError(3, numOfLine);
                        return 0;
                    }
                }
            }return 1;
        }
    }return 1;
}

void checkWord()
{
    int i=0;
    space
    while (line[place]!=' '&&line[place]!='\t'&&line[place]!='\r'&&line[place]!='\n')
    {

        word[i]=line[place];
        place++;
        i++;
    }
    word[i] = '\0';
}

int careData(char* preWord)
{
    int proper = 0;
    char digits[80];
    int i = 0;
    int j;
    int num;
    
    
    if (isLabel)
    {
        proper=addSymbol(preWord, DC+1, 'd');
        if (!proper)
        {
            printError(4, numOfLine);
            return 0;
        }
    }
    
    
    while (place<MAXLEN && (line[place]!='\n') ) {
        space
        i=0;
        while ((line[place]!=',')&&(place<MAXLEN) && (line[place]!='\n')) {
            
            digits[i] =line[place];
            i++;
            place++;
        }
        digits[i] ='\0';
        if (line[place]!='\n') place++;/*for ',' car*/
        
        if ((digits[0] < '0' || digits[0] > '9')&& (digits[0]!='+'&&
                                                    digits[0]!='-') )
        {
            printError(5, numOfLine);
            return 0;
        }
        for (j=1; j<i; j++) {
            if (digits[j] < '0' || digits[j] > '9')
            {
                printError(5, numOfLine);
                return 0;
            }
        }
        num=atoi(digits);
        proper = insertData(num);
        if(!proper) return  proper;
    }
    return proper;
}
    
    
int careString(char* preWord)
    {
        int proper=1;
        char stringWord [80];
        int i;
        int endOfString=0;
        if (isLabel)
        {
            proper=addSymbol(preWord, DC, 'd');
            if (!proper)
            {
                printError(4, numOfLine);
                return 0;
            }
        }
        space
        if (line[place]!='"')
        {
            printError(6, numOfLine);
            return 0;
        }
        endOfString=(place+1);
        
        while ((line[endOfString]!='"')&&(endOfString<MAXLEN)&&(line[endOfString]!='\n')) {
            endOfString++;
        }
        if (line[endOfString]!='"')
        {
            printError(6, numOfLine);
            return 0;
        }
        for (place+=1,i=0; place<endOfString; place++, i++) {
            stringWord[i]=line[place];
        }
        stringWord[i] ='\0';
        
        proper = insertString(stringWord);
        
        return proper;
    }

int careInstruction(char* preWord)
{
    int j;
    int proper=1;
    char code[13];
    char *numberC;
    char *address;
    int number;
    int numOpSrc;
    int numOpDes = 0;
    char *binrSrc = NULL;
    char *binrDes = NULL;
    int thereIsComma = 0;
    char tmpWord[32];
    
    memset(tmpWord, '\0', strlen(tmpWord));
    
    p=(operation *)malloc(sizeof(operation));
    
    if (isLabel)
    {
        proper=addSymbol(preWord, IC, 'c');
        if (!proper)
        {
            printError(4, numOfLine);
            return proper;
        }
    }
    getOeration(word,p);
    if(p->name == NULL)
    {
        proper =0;
        printError(8, numOfLine);
        return proper;
    }
    space
    /*Treatment of none operands instrution.*/
    if ((p->opSourc[0]==-1)&&(p->opDes[0]==-1))
    {
        while (line[place]!='\n') {
            if ((line[place]!=' ')&&(line[place]!='\t')&&(line[place]!='\r')){
                printError(8, numOfLine);
                return 0;
            }
            place++;
        }
        address="0000";
        strncpy(code, p->code, strlen(p->code)+1);
        strcat(code, address);
        proper = insertInstruction(code);
        IC++;
        return  proper;
    }
/*Treatment of one operand's instructions*/
    if(p->opSourc[0] == -1)
    {
        space
        
        checkWord();/*read the destination*/
        /*define code for any address method*/
        switch (word[0]) {
            case  '#':
                numOpDes = 0;
                binrDes= "00";
                break;
            case  '%':
                numOpDes = 2;
                binrDes= "10";
                break;
                
            case  'r':
                if ((word[1] >= '0') &&( word[1]  <= '7')){
                    numOpDes = 3;
                    binrDes= "11";
                }
                else
                {
                    /*it is lable*/
                    numOpDes= 1;
                    binrDes= "01";
                }
                break;
                
            default:
                numOpDes= 1;
                binrDes= "01";
                break;
        };
        /*check if address character match to number of address.*/
        for (j=0; j<3; j++) {
            if (p->opDes[j]==numOpDes)
                j=3;
        }
        if (j!=4) {
            printError(8, numOfLine);
            return 0;
        }
        
        
        while (line[place]!='\n') {
            if ((line[place]!=' ')&&(line[place]!='\t')&&(line[place]!='\r')){
                printError(8, numOfLine);
                return 0;
            }
            place++;
        }
        
        address="00";
        strncpy(code, p->code, strlen(p->code)+1);
        strcat(code, address);
        strcat(code, binrDes);
        
        proper = insertInstruction(code);
        IC++;
        if(!proper) return proper;
        
        if(numOpDes == 0){
            numberC = word +1;
            number = atoi(numberC);
            if (!number) {
                printError(8, numOfLine);
                return 0;
            }
            convBinry(code, number);
            proper = insertInstruction(code);
            IC++;
            return proper;
        }
        if (numOpDes == 3)
        {
            getRgisterCode(atoi(&word[1]), code);
            proper = insertInstruction(code);
            IC++;
            return proper;
        }
        
        /*check if the label is a reserved word*/
        strncpy(tmpWord,word,strlen(word)-1);
        for (j=0;j<28;j++)/*for 28 savedwords*/
        {
            if (tmpWord==reservedWord[j])
            {
                printError(3, numOfLine);
                return 0;
            }
        }
        code[0] ='?';
        code[1] = '\0';
        proper = insertInstruction(code);
        IC++;
        return proper;
    }
    
    /*=======if there is opSrc and opDes*/
    space
    checkWord();/*read the source*/
    /* define code for any address method*/
        switch (word[0]) {
            case  '#':
                numOpSrc = 0;
                binrSrc= "00";
                break;
            case  'r':
                if ((word[1] >= '0') &&( word[1]  <= '7')){
                    numOpSrc = 3;
                    binrSrc= "11";
                }
                else
                {
                    /*it is lable*/
                    numOpSrc= 1;
                    binrSrc= "01";
                }
                break;
                
            default:
                numOpSrc= 1;
                binrSrc= "01";
                break;
        };
         /*check if address character match to number of address.*/
        for (j=0; j<3; j++) {
            if (p->opDes[j]==numOpSrc)
                j=3;
        }
        if (j!=4) {
            printError(8, numOfLine);
            return 0;
        }
    
    /*================check ,=========*/
    if(word[strlen(word)-1]==',') /*in the last char of opSrc*/
        thereIsComma = 1;
    
    strcpy(preWord, word);
    space
    checkWord();/*read the destination or comma*/
    
    
    if(!thereIsComma && word[0]==',' && strlen(word) == 1){/*between space*/
        thereIsComma = 1;
        space
        checkWord();/*read the destination*/
    }
    if(!thereIsComma && word[0]==',' && strlen(word) > 1){/*in the firs char of opDes*/
        thereIsComma = 1;
    }
    
    if(!thereIsComma){/*error not exist comma*/
        printError(8, numOfLine);
        return 0;
    }
/* define code for any address method*/
    switch (word[0]) {
        case  '#':
            numOpDes = 0;
            binrDes= "00";
            break;
        case  'r':
            if ((word[1] >= '0') &&( word[1]  <= '7')){
                numOpDes = 3;
                binrDes= "11";
            }
            else
            {
                /*it is lable*/
                numOpDes= 1;
                binrDes= "01";
            }
            break;
            
        default:
            numOpDes= 1;
            binrDes= "01";
            break;
    };
      /* check if address character match to number of address.*/
    for (j=0; j<3; j++) {
        if (p->opDes[j]==numOpDes)
            j=3;
    }
    if (j!=4) {
        printError(8, numOfLine);
        return 0;
    }
    
    
    while (line[place]!='\n') {
        if ((line[place]!=' ')&&(line[place]!='\t')&&(line[place]!='\r')){
            printError(8, numOfLine);
            return 0;
        }
        place++;
    }
    
    strncpy(code, p->code, strlen(p->code)+1);
    strcat(code, binrSrc);
    strcat(code, binrDes);
    
    proper = insertInstruction(code);/*for operation*/
    IC++;
    if(!proper) return proper;
    
    if(numOpSrc == 0){
        numberC = preWord +1;
        number = atoi(numberC);
        if (!number) {
            printError(8, numOfLine);
            return 0;
        }
        convBinry(code, number);
        proper = insertInstruction(code);/*for opSrc*/
        IC++;
        if(!proper) return proper;
        
    }
    if (numOpSrc == 3)
    {
        getRgisterCode(atoi(&preWord[1]), code);
        code[12] = '\0';
        proper = insertInstruction(code); /*for opSrc*/
        IC++;
        if(!proper) return proper;
        
    }
    if(numOpSrc == 1){
      /*check if the label is a reserved word*/
        strncpy(tmpWord,preWord,strlen(preWord)-1);
        for (j=0;j<28;j++)/*for 28 savedwords*/
        {
            if (tmpWord==reservedWord[j])
            {
                printError(3, numOfLine);
                return 0;
            }
        }
        code[0] ='?';
        code[1] = '\0';
        proper = insertInstruction(code);/*for opSrc*/
        IC++;
        if(!proper) return proper;
    }
    
    if(numOpDes == 0){
        numberC = word +1;
        number = atoi(numberC);
        if (!number) {
            printError(8, numOfLine);
            return 0;
        }
        convBinry(code, number);
        proper = insertInstruction(code);/*for opDes*/
        IC++;
        return proper;
    }
    if (numOpDes == 3)
    {
        getRgisterCode(atoi(&word[1]), code);
        proper = insertInstruction(code);/*for opDes*/
        IC++;
        return proper;
    }
    if(numOpDes == 1){
       /*check if the label is a reserved word*/
        strncpy(tmpWord,word,strlen(word)-1);
        for (j=0;j<28;j++)/*for 28 savedwords*/
        {
            if (tmpWord==reservedWord[j])
            {
                printError(3, numOfLine);
                return 0;
            }
        }
        code[0] ='?';
        code[1] = '\0';
        proper = insertInstruction(code);/*for opDes*/
        IC++;
        return proper;
    }
    return proper;
}


