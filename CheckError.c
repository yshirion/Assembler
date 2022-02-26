
#include <stdio.h>
#include <string.h>
#include "CheckError.h"

void printError(int code,int numOfLine)/*print error foreach mistake*/
{
    char *err = NULL;
    
    switch (code) {
        case 1:
            err="Error: the line is larger than 80 character.\n";
            break;
        case 2:
            err="Error: the label is larger than 31 character.\n";
            break;
        case 3:
            err="Error: the label is a save word.\n";
            break;
        case 4:
            err="Error: the label is alredy exist in the symbols table.\n";
            break;
        case 5:
            err="Error: the syntax of 'data' is wrong.\n";
            break;
        case 6:
            err="Error: the syntax of 'string' is wrong.\n";
            break;
        case 7:
            err="Warning: label here is mistake.\n";
            break;
        case 8:
            err="Error: the instruction is incorrect.\n";
            break;
        case 9:
            err="Error: the label isn't exists.\n";
            break;
        default:
            break;
    }
    printf("In line %d: %s\n", numOfLine, err);
}



