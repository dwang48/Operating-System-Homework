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



using namespace std;

//For read in files
//vector<vector<string>> tokens_list; //tokens_list[i] is the (i-1)th row, where tokens_list[i][j] is the jth element
vector<string> symbols;

//hashmap and hashset for fast lookup
unordered_set<char> instructions = {'R','E','I','A'};
unordered_set<string> symbol_set;
unordered_map<int,int> symbol_module_map;
unordered_map<int,int> module_offset_map;
unordered_map<string,int> symbol_val_map;
unordered_map<string,pair<int,int>> symbol_location_map;


//For print parse errors
int linenum,lineoffset,linelen;
int current_token = 0;
int total_tokens;
int errcode = -1;
int warncode = -1;

int num_instr = 0;
vector<string> instructions_list;




//functions list for the assignment
//convert token to int
int converter(string input);
//parse input
void getToken(char* argv[]);
string definition_parser(string input);
string use_parser(string input);
string program_parser(string input);

//functions that help check that the tokens have the correct sequence of characters and length
bool isInt(string input);
bool isSymbol(string input);
bool readIAER(string input);
int readInt();
int readSymbol();
int readIAER();


//4 instructions
string relative(string input,string error);
string external(string input, string error);
string immediate(string input, string error);
string absolute(string input, string error);


//functions for error messages and warning messages
int generate_error_code();
int generate_warning_code();
void __parseerror(int errcode);
void __parsewarning(int warncode);

//two passes
void pass1();
void pass2();

//generate output
void generate_symbol_table();
void generate_memory_map();

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

string readSymbol(){

}


bool isInt(string input){
  for(int i = 0; i < input.size(); i++){
    if(!isdigit(input[i])){
      return false;
    }
  }
  return true;
}

string readInt(){

}

bool readIAER(string input){
  if(input.size()==1&&(instructions.find(input[0])!=instructions.end())){
    return true;
  }
  else{
    return false;
  }
}



string getToken(char* argv[],int linelen, int linenum){
  string token;
  string line;
  ifstream input(argv[1]);
  if(input.is_open()){
    while(getline(input,line,'\n')){
      linenum++;
      istringstream iss(line);
      string current;
      while(getline(iss,current,' ')){
        if(isSymbol(current)&&!symbol_set.count(current)){
          symbols.push_back(current);
          symbol_set.insert(current);
        }
        if(readIAER(current)){
            num_instr++;
            instructions_list.push_back(current);
        }
      }
    }
  input.close();
  }
  else{
    cout<<"Unable to open file";
  }
}

void pass1(int num,char* argv[], vector<string>& symbols){
  int module  = 1;
  int module_base = 0;


  int linenum = 0;
  int lineoffset = 1;

  ifstream file(argv[1]);
  int i = 1;
  string line;
  if(file.is_open()){
    int defcount = readInt();
    for(int i = 0; i < defcount;i++){
      string symbol = readSymbol();
      int val = readInt();
      createSymbol(symbol,val);
    }
    int usecount = readInt();
    for(int i = 0;i <usecount; i++){
      string symbol = readSymbol();
    }
    int instcount = readInt();
    for(int i = 0; i < )

  }
  file.close();
}

void pass1(int num,char* argv[], vector<string>& symbols){
  int module  = 1;
  int module_base = 0;


  int linenum = 0;
  int lineoffset = 1;

  ifstream file(argv[1]);
  int i = 1;
  string line;
  if(file.is_open()){
    int defcount = readInt();
    for(int i = 0; i < defcount;i++){
      string symbol = readSymbol();
      int val = readInt();
      createSymbol(symbol,val);
    }
    int usecount = readInt();
    for(int i = 0;i <usecount; i++){
      string symbol = readSymbol();
    }
    int instcount = readInt();
    for(int i = 0; i < )

  }
  file.close();
}

void pass2(int num,char* argv[], vector<string>& symbols){
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

string relative(string input,string error){
  int op = stoi(input);
  if(op>9999){
    op = 9999;
    error = "Error: Illegal opcode; treated as 9999";
  }
  int opcode = op/1000;
  int operand = op%1000;


  return to_string(op);
}

string external(string input,string error){
  int op = stoi(input);
  if(op>9999){
    op = 9999;
    error = "Error: Illegal opcode; treated as 9999";
  }
  int opcode = op/1000;
  int operand = op%1000;

  return to_string(op);
}


string immediate(string input, string error){
  if(input.size()>4){
    error = " Error: Illegal immediate value; treated as 9999";
    return "9999";
  }
  else{
    int address = stoi(input);
  }

  return input;
}


string absolute(string input, string error){
  string error = "";
  int op = stoi(input);
  if(op>9999){
    op = 9999;
    error = "Error: Illegal opcode; treated as 9999";
  }
  int opcode = op/1000;
  int operand = op%1000;


  return to_string(op);
}



void __parseerror(int errcode) { 
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


void generate_symbol_table(){
  cout<<"Symbol Table"<<endl;
  for(int i = 0; i < symbols.size();i++){
    cout<<symbols[i]<<"="<<endl;
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



int main (int argc, char* argv[])
{
  if(argc !=2){
    cout<< "One input file is required."<<endl;
    exit(EXIT_FAILURE);
  }
  getToken(argv);
  



  pass1(argv,symbols);

  pass2();
  
//   for(int i = 0; i < num_instr;i++){
//     cout<<instructions_list[i]<<endl;
//   }

  return 0;
}