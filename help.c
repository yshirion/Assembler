

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "help.h"
#include "Consts.h"
#include "FirstSkim.h"


extern int ICF;
extern int IDF;


void convBinry (char binar[],int num)/*convert number to binary in 2 bits*/
{
    int j;
    int i=11;
    int absNum = 0;
    
    if (num<0)
        absNum=num*(-1);
    else
        absNum=num;
    
    for(j=0; j<12; j++)
    {
      binar[j]= '0';
    }
    binar[12] ='\0';
   while (absNum>0)
   {
       if(absNum % 2 != 0)
           binar[i]='1';
       absNum=absNum/2;
       i--;
   }
    if (num<0)/*tritment to negative number*/
    {
        for (i=11; i>=0; i--) {
            if (binar[i]=='1')
                break;
        }
        for (i-=1; i>=0; i--) {
            if (binar[i]=='0')
                binar[i]='1';
            else binar[i]='0';
        }
    }
   
   
}
void convHexa(FILE *f, char* bin)/*convert binary number to hexa*/
{

    char *a = bin;
    int num = 0;
    do {
        int b = (*a=='1')?1:0;
        num = (num<<1)|b;
        a++;
    } while (*a);
    fprintf(f,"%03X\t", num);
}

void printOb(FILE *f)/*print .ob file all the codeImage*/
{
    image *head=NULL;
    
    fprintf(f,"%4d  %d\n", ICF, IDF);
    
    head=getHeadCodeImage();
    while (head!=NULL)
    {
        fprintf(f,"%04d\t", head->adress);
        convHexa(f,head->coding);
        fprintf(f, "%c\n", head->type);
        head=head->next;
    }
}

void printEnt(FILE *f)/*print .ent file all entry labels */
{
    int i;
    symbol *head=NULL;
    
    head=getHeadSymbolTable();
    while (head!=NULL)
    {
        if (head->atrr[0]=='n'||head->atrr[1]=='n') {
            for (i=0;i<strlen( head->name);i++)
            {
                fprintf(f, "%c",head->name[i]);
            }
            fprintf(f, "\t%04d\n", head->value);
            
        }
        head=head->next;
    }
}

void printExt(FILE *f)/*print .ent file all external labels */
{

    externs *head=NULL;
    
    head=getHeadExternTable();
    while (head!=NULL)
    {

        fprintf(f, "%s\t%04d", head->name,head->address);
        head=head->next;
    }
}
