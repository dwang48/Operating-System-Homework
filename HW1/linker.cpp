#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>
#include <map>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iterator>
#include <unordered_set>
#include <unordered_map>
#include <regex>
#include <limits.h>



using namespace std;

//For read in files
//vector<vector<string>> tokens_list; //tokens_list[i] is the (i-1)th row, where tokens_list[i][j] is the jth element
vector<string> symbols;

//hashmap and hashset for quick lookup
unordered_set<char> instructions = {'R','E','I','A'};
unordered_map<string,int> symbols_table;


//For print parse errors
int errcode = -1;
int warncode = -1;

int num_instr = 0;
vector<string> instructions_list;


void getNewLine(FILE* file, char* line, int& linelen, int& linenum) {
    if (fgets(line, INT_MAX, file) != NULL){
        linenum++;
        if (strlen(line) != 0)
        {
          linelen = strlen(line);
        } else {
          getNewLine(file, line, linelen, linenum);
        }
    } 
}


char* getToken(char* input){
  char* token = strtok(input,"\t\n");
  return token;  
} 
  // if(input.is_open()){
  //     getline(input,line,'\n');
  //     if(line!=""){
  //       linenum++;
  //       istringstream iss(line);
  //       string current;
  //       lineoffset = -1;
  //       while(getline(iss,current,' ')){
  //         lineoffset++;
  //         // if(isSymbol(current)&&!symbol_set.count(current)){
  //         //   symbols.push_back(current);
  //         //   symbol_set.insert(current);
  //         // }
  //         // if(isIAER(current)){
  //         //     num_instr++;
  //         //     instructions_list.push_back(current);
  //         // }
  //         token = current;
  //         return token;
  //       }
  //     }
  // input.close();
  // }
  // else{
  //   cout<<"Unable to open file";
  // }
//   return token;
// }

bool isSymbol(string input){
  char c = input[0];
  
  //check is the input is a number or an instruction
  if(!isalpha(c)){
    return false;
  }
  else if(instructions.find(c)!=instructions.end()){
    return false;
  }
  return true;
}

string readSymbol(FILE* file,char* line, int& linelen, int& linenum, int& lineoffset){
  char* token = getToken(file,line,linelen,linenum,lineoffset);
  cout<<token;
  if(isSymbol(token)){
    return token;
  }
  else{
    return "";
  }
}


bool isInt(char* input){
  for(int i = 0; i < strlen(input); i++){
    if(!isdigit(input[i])){
      return false;
      cout<<input[i];
    }
  }
  return true;
}

int readInt(char* input){
  char* token = getToken(input);
  if(token==NULL){
    return -1;
  }
  else if(isInt(token)){
    return atoi(token);
  }
  else{
    return INT_MIN;
  }
}

bool isIAER(char* input){
  if(strlen(input)==1&&(instructions.find(input[0])!=instructions.end())){
    return true;
  }
  else{
    return false;
  }
}

char readIAER(char* input){
  char* token = getToken(input);
  cout<<token;
  if(isIAER(token)){
    return input[0];
  }
  else{
    return 'n';
  }
}


string relative(string input,string& error){
  int op = stoi(input);
  if(op>9999){
    op = 9999;
    error = "Error: Illegal opcode; treated as 9999";
  }
  int opcode = op/1000;
  int operand = op%1000;


  return to_string(op);
}

string external(string input,string& error){
  int op = stoi(input);
  if(op>9999){
    op = 9999;
    error = "Error: Illegal opcode; treated as 9999";
  }
  int opcode = op/1000;
  int operand = op%1000;

  return to_string(op);
}


string immediate(string input, string& error){
  if(input.size()>4){
    error = " Error: Illegal immediate value; treated as 9999";
    return "9999";
  }
  else{
    int address = stoi(input);
  }

  return input;
}


string absolute(string input, string& error){
  int op = stoi(input);
  if(op>9999){
    op = 9999;
    error = "Error: Illegal opcode; treated as 9999";
  }
  int opcode = op/1000;
  int operand = op%1000;


  return to_string(op);
}


int pass1(char* argv[]){
  int module  = 1;
  int module_base = 0;

  ifstream file(argv[1]);

  int linelen;
  int linenum = 0;
  int lineoffset = 1;
  char line[LINE_MAX];

  char* line1;
  getline(file,line1);

  int defcount = readINT(line1);
  if(file.is_open()){
    string line;
    while(getline(file))
    int defcount = readInt(file,line,linelen,linenum,lineoffset);
    cout<<defcount;
    for(int i = 0; i < defcount;i++){
      string symbol = readSymbol(file,line,linelen,linenum,lineoffset);
      int val = readInt(file,line,linelen,linenum,lineoffset);

      symbols_table[symbol] = val+lineoffset;
    }
  }
  //   int usecount = readInt(file,line,linelen,linenum,lineoffset);
  //   for(int i = 0;i <usecount; i++){
  //     string symbol = readSymbol(file,line,linelen,linenum,lineoffset);
  //   }

  //   int instcount = readInt(file,line,linelen,linenum,lineoffset);
  //   num_instr+= instcount;
  //   for(int i = 0; i < instcount; i++){
  //     string mode = readIAER(file,line,linelen,linenum,lineoffset);
  //     string num = to_string(readInt(file,line,linelen,linenum,lineoffset));
  //     string error = "";
  //     if(mode =="I"){
  //       immediate(num,error);
  //       if(error!="")
  //         cout<<error<<endl;
  //         return 1;
  //     }
  //     else if(mode == "A"){
  //       absolute(num,error);
  //       if(error!="")
  //         cout<<error<<endl;
  //         return 1;
  //     }
  //     else if(mode == "E"){
  //       external(num,error);
  //       if(error!="")
  //         cout<<error<<endl;
  //         return 1;
  //     }
  //     else{
  //       relative(num,error);
  //       if(error!="")
  //         cout<<error<<endl;
  //         return 1;
  //     }
  //   }
  // }
  return 0;
}

void generate_symbol_table(){
  cout<<"Symbol Table"<<endl;
  for(int i = 0; i < symbols.size();i++){
    cout<<symbols[i]<<"="<<symbols_table[symbols[i]]<<endl;
  }
}


void generate_memory_map(){
  cout<<"Memory Map"<<endl;
  for(int i = 0; i < num_instr; i++){
    if(i<10){
        cout<<"00"+to_string(i)+":"<<endl;
    }
    else if(i>=0&&i<100){
        cout<<"0"+to_string(i)+":"<<endl;
    }
    else{
        cout<<to_string(i)+":"<<endl;
    }
  }
}

void pass2(FILE* file){
  generate_symbol_table();
  generate_memory_map();
}


int converter(string input){
  if(input.size()>4){
    return 9999;
  }
  else{
    return stoi(input);
  }
}

// vector<vector<string>> getToken(vector<string>& tokens){
//   int n = tokens.size();
//   tokens_list.resize(n);
//   for(int i = 0; i < n; i++){
//     istringstream iss(tokens[i]);
//     string current;
//     while(getline(iss,current,' ')){
//       tokens_list[i].push_back(current);
//     }
//   }
//   return tokens_list;
// }


void __parseerror(int errcode,int& linenum, int& lineoffset) { 
    vector<string> errors = { 
        "NUM_EXPECTED",        //0 Number expect, anything >= 2^30 is not a number either 
        "SYM_EXPECTED",   //1 Symbol Expected 
        "ADDR_EXPECTED",  //2 Addressing Expected which is A/E/I/R   
        "SYM_TOO_LONG",  //3 Symbol Name is too long 
        "TOO_MANY_DEF_IN_MODULE", //4  > 16  
        "TOO_MANY_USE_IN_MODULE",     //5  > 16 
        "TOO_MANY_INSTR"             //6  total num_instr exceeds memory size (512)   
    }; 
    if(errcode = -1){
      cout<<"No Error!";
    }
    else{
      cout<<"Parse Error line"<<linenum<< "offset"<< lineoffset <<":"<<errors[errcode]<<endl; 
    }
}

// void __parsewarning(int warncode){
//   if(warncode = -1){
//     cout<<"No Warning";
//   }
//   else{
//     cout<<
//   }
// }




int main (int argc, const char* argv[])
{
  if(argc !=2){
    cout<< "One input file is required."<<endl;
    exit(EXIT_FAILURE);
  }
  
  FILE *file = fopen(argv[1],"r");
  int ret = pass1(file);
  fclose(file);

  // if(ret==1){
  //   return 1;
  // }
  // else{
  //   file = fopen(argv[1],"r");
  //   pass2(file);
  //   fclose(file);
  // }
  return 0;


  //pass2(argv);
  
//   for(int i = 0; i < num_instr;i++){
//     cout<<instructions_list[i]<<endl;
//   }

  return 0;
}