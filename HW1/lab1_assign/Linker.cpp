#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>
#include <map>
#include <algorithm>
using namespace std;


int linenum,lineoffset;

//Function list for the Lab
void read();





void __parseerror(int errcode) { 
    static char* errstr[] = { 
        "NUM_EXPECTED",        // Number expect, anything >= 2^30 is not a number either 
        "SYM_EXPECTED",   // Symbol Expected 
        "ADDR_EXPECTED",  // Addressing Expected which is A/E/I/R   
        "SYM_TOO_LONG",  // Symbol Name is too long 
        "TOO_MANY_DEF_IN_MODULE", // > 16  
        "TOO_MANY_USE_IN_MODULE",     // > 16 
        "TOO_MANY_INSTR",             //total num_instr exceeds memory size (512)   
    }; 
    printf("Parse Error line %d offset %d: %s\n", linenum, lineoffset, errstr[errcode]); 
}


int main ()
{
  char str[] ="- This, a sample string.";
  char * pch;
  printf ("Splitting string \"%s\" into tokens:\n",str);
  pch = strtok (str," ,.-");
  while (pch != NULL)
  {
    printf ("%s\n",pch);
    pch = strtok (NULL, " ,.-");
  }
  return 0;
}
