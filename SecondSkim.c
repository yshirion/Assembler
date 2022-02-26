
#include <stdio.h>
#include <string.h>
#include "SecondSkim.h"
#include "CheckError.h"
#include "Consts.h"
#include "help.h"

#define space while (line2[place2]==' '||line2[place2]=='\t') place2++;
#define MAXLEN 80

static char line2[81];
static char word2 [32];
int numOfLine2;
int isLabel2=0;
int place2=0;
int newIc=100;
operation *p;

int doSecondSkim (FILE *f)
{
    int res;
    int proper=1;
    const char endl = '\n';
    char *ret = NULL;
    char c;

    numOfLine2 = 0;
    while(fgets(line2, sizeof(line2), f) != NULL)
    {
        numOfLine2 ++;
        /*here we need to check line*/
        
        
        /*Here we check if the length of line is proper, and find '\n'.*/
        ret = strchr(line2, endl);
        if (!ret)
        {
            printError(1, numOfLine2);
            while ((c=getc(f))!=EOF)
            {
                if (c=='\n')
                    break;
            }
        }
        
        res = checkLine2();
        if (!res) proper=0;
        memset(line2,'\0',(int)sizeof(line2));
    }
    
  
    return proper;
}

int checkLine2()
{
    int proper = 1;
    place2 = 0;
    isLabel2=0;
    space
    if (line2[place2]==';'||line2[place2]=='\n') {
        return 1;
    }
    
    
    checkWord2();
    
    if (word2[place2-1]==':')/*if it lable*/
    {
        isLabel2=1;
        proper=careLine2();
        return proper;
    }
    space
    
    place2 = place2-(int)strlen(word2)-1;
    proper=careLine2();
    /*if (!proper) return 0;*/
    return proper;
}
void checkWord2()/*read word*/
{
    int i=0;
    space
    while (line2[place2]!=' '&&line2[place2]!='\t'&&line2[place2]!='\r'&&line2[place2]!='\n')
    {

        word2[i]=line2[place2];
        place2++;
        i++;
    }
    word2[i] = '\0';
}


int careLine2()
{
    int proper;
    int thereIsComma=0;
    
    checkWord2();
	

    if(!(strcmp(word2, ".data")) ||!(strcmp(word2, ".string"))||!(strcmp(word2, ".extern")))
        return 1;
    
    if (!(strcmp(word2, ".entry")))
    {
        checkWord2();
        proper = checkSymbol(word2);
        if (!proper){
            printError(9, numOfLine2);
            return proper;}
        addSymbol(word2, 0, 'n');
        return 1;
    }
    /*tritment instraction */
    getOeration(word2,p);
    
   /*Treatment of none operands instrution.*/
    newIc++;/* increase for operation.*/
    /*case-none operands.*/
    if ((p->opSourc[0]==-1)&&(p->opDes[0]==-1))
        return 1;
    /*case-one operand.*/
        if(p->opSourc[0] == -1)
        {
            checkWord2();
            
            proper=updateLabel();
            return proper;
    }
    /*case-two operands.*/
    checkWord2();
    if (word2[strlen(word2)-1]==','){
        thereIsComma=1;
        word2[strlen(word2)-1]='\0';
    }
    proper=updateLabel();
    if (thereIsComma)
    {
        checkWord2();
        proper=updateLabel();
    } else{
        checkWord2();
        if (!strcmp(word2, ","))
        {
                checkWord2();
                proper=updateLabel();
        } else proper=updateLabel();
    }
    return  proper;
}
int updateLabel()
{
    int proper=0;
    char *tmpWord;
    char code[13];
    int value;
    
    if (word2[0]=='#'){
        newIc++;
        return 1;
    }
    if ((word2[0]=='r')&&((word2[1] >= '0') &&( word2[1]  <= '7')))
    {
        newIc++;
        return 1;
    }
    if(word2[0]==',')
    {
        if (word2[1]=='%')
            tmpWord=word2+2;
        else 
	tmpWord=word2+1;
    }
   else
	 if (word2[0]=='%') 
		tmpWord=word2+1;
          else 
		tmpWord = word2;
    proper = checkSymbol(tmpWord);
    if (!proper){
        printError(9, numOfLine2);
        return proper;}
    value = getValueSymbol(tmpWord);
    convBinry(code, value);
    
    proper = updateCodeLable(newIc, code, value);
    if (!value)
        proper=addExtern (tmpWord, newIc);
    newIc++;
    return proper;
}
