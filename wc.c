0
nclude <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

void wc ( FILE *infile, char *inname);
int totWord=0;
int totChar=0;
int totLine=0;

int main (int argc, char *argv[]) {
  int i=1;
  if(argc>1)
    {
      for(;i<argc;i++)
	{
          FILE *file = fopen(argv[i],"r");
          wc(file, argv[i]);
	}
    }

  printf ("%3d %6d %6d %3s\n", totLine, totWord, totChar, "total"); 

    
}

void wc( FILE *infile, char *inname) {

   int countLine = 0;
   int countWord=0;
   int countChar=0;
   char ch;
   char ch2;

   while ((ch = fgetc(infile)) != EOF){

   countChar++;

       if (ch == '\n')
           countLine++;
   }

   fclose(infile);
   infile = fopen(inname,"r");

 int r=0;

 while ((r = fscanf(infile, "%*100s")) != EOF)
   {
     countWord++;
   }

 totLine = totLine+countLine;
 totWord = totWord+countWord;
 totChar = totChar + countChar;


 printf ("%3d %6d %6d %3s\n", countLine, countWord, countChar, inname);

}
