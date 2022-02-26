#include <stdio.h>
#include <string.h>
#include "Consts.h"
#include "FirstSkim.h"
#include "SecondSkim.h"
#include "help.h"
#include "CheckError.h"



int main (int argc, char *argv[])
{
    
    char *nameOfFile=NULL;
    char *fileOb=NULL;
    char *fileEnt=NULL;
    char *fileExt=NULL;
    int resultFirstSkim;
    int resultSecondSkim=0;
	FILE *f=NULL;
       
	
    while ((argc--)>1)
    {
       nameOfFile=argv[argc];
	
	strcat(nameOfFile,".as");
	
       printf("Treatment of file %s:\n", nameOfFile);
       f = fopen(nameOfFile, "rt");
        if(f==NULL)
        {
            printf("The file %s doesnt exist\n", nameOfFile);
           continue;
        }
        resultFirstSkim= doFirstSkim(f);
    fclose(f);
   
    if (resultFirstSkim)
    {
        f = fopen(nameOfFile , "rt");
        resultSecondSkim=doSecondSkim(f);
        fclose(f);
	
    }
    if (resultSecondSkim)
    {
	/*.ob file*/
      fileOb=strcat(argv[argc],".ob");
        f = fopen(fileOb, "wt");
        printOb(f);
        fclose(f);
	/*.ent file*/
       fileEnt=strcat(argv[argc],".ent");
        f = fopen(fileEnt , "wt");
        printEnt(f);
        fclose(f);
	/*.ext file*/
       fileExt=strcat(argv[argc],".ext");
        f = fopen(fileExt , "wt");
        printExt(f);
         fclose(f);
    }
        
        /*free objects*/
        freeSymbol();
   	 freeImage();
    
    }
    return 0;
}

