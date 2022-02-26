

#ifndef FirstSkim_h
#define FirstSkim_h

#include <stdio.h>



int doFirstSkim (FILE *f);
int checkLine(void);
int checkLabel (void);
int careLine(void);
void checkWord(void);
int careString(char* preWord);
int careData(char* preWord);
int careInstruction(char* preWord);


#endif /* FirstSkim_h */
