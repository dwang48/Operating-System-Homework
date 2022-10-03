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
int linenum,lineoffset;
int current_token = 0;
int total_tokens;
int errcode = -1;
int warncode = -1;





struct definition_list{
  int defcount = 0;
  vector<pair<string,string>> deflist;
};

struct use_list{
  int usecount = 0;
  vector<string> uselist;
};

struct program_text{
  int codecount = 0;
  vector<pair<char,int>> text;
};




//functions list for the assignment
void read(char* argv[]);
//convert token to int
int converter(string input);
//parse input
void getToken(char* argv[]);
string definition_parser(string input);
string use_parser(string input);
string program_parser(string input);

//functions that help check that the tokens have the correct sequence of characters and length
bool readInt(string input);
bool readSymbol(string input);
bool readIAER(string input);


//4 instructions
void relative(string input);
void external(string input);
void immediate(string input, int idx);
void absolute();


//functions for error messages and warning messages
int generate_error_code();
int generate_warning_code();
void __parseerror(int errcode);
void __parsewarning(int warncode);

//two passes
void pass(int num);//num = 1 for pass1, num = 2 for pass 2


//generate output
void generate_symbol_table();
void generate_memory_map();

bool readSymbol(string input){
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

bool readInt(string input){
  for(int i = 0; i < input.size(); i++){
    if(!isdigit(input[i])){
      return false;
    }
  }
  return true;
}

bool readIAER(string input){
  if(input.size()==1&&(instructions.find(input[0])!=instructions.end())){
    return true;
  }
  else{
    return false;
  }
}



void getToken(char* argv[]){
  string line;
  ifstream input(argv[1]);
  if(input.is_open()){
    int row = 0;
    while(getline(input,line,'\n')){
      row++;
      istringstream iss(line);
      string current;
      int col = -1;
      while(getline(iss,current,' ')){
        col++;
        if(readSymbol(current)&&!symbol_set.count(current)){
          symbols.push_back(current);
          symbol_set.insert(current);
          symbol_location_map[current] = make_pair(row,col);
        }
      }
    }
  input.close();
  }
  else{
    cout<<"Unable to open file";
  }
}

void pass(int num,char* argv[], vector<string>& symbols){
  int module  = 1;
  int module_base = 0;

  int instruction_num = 0;
  int linenum = 0;
  int lineoffset = 1;

  ifstream file(argv[1]);
  int i = 1;
  string line;
  if(file.is_open()){
    cout<<i++;
  }
  file.close();
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

void relative(string input){
  string error = "";
  int op = stoi(input);
  if(op>9999){
    op = 9999;
    error = "Error: Illegal opcode; treated as 9999";
  }
  int opcode = op/1000;
  int operand = op%1000;
}

void external(string input){
  string error = "";
  int op = stoi(input);
  if(op>9999){
    op = 9999;
    error = "Error: Illegal opcode; treated as 9999";
  }
  int opcode = op/1000;
  int operand = op%1000;
}


void immediate(string input, int idx){
  if(input.size()>4){
    cout<<" Error: Illegal immediate value; treated as 9999" << endl;
  }
  else{
    int address = stoi(input);
  }
}


void absolute(string input){
  string error = "";
  int op = stoi(input);
  if(op>9999){
    op = 9999;
    error = "Error: Illegal opcode; treated as 9999";
  }
  int opcode = op/1000;
  int operand = op%1000;
}


// int generate_errcode(int current_token){
//   if(current_token >= pow(2,30)){
//     return 0;
//   }
  

//   if(defcount>16){
//     return 5;
//   }
//   if(usecount>16){
//     return 6;
//   }
  
//   return -1;
// }


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
}


void generate_memory_map(){
  cout<<"Memory Map"<<endl;
}



int main (int argc, char* argv[])
{
  if(argc !=2){
    cout<< "One input file is required."<<endl;
    exit(EXIT_FAILURE);
  }
  getToken(argv);
  



  pass(1,argv,symbols);

  //pass(2,argv);
  


  // generate_symbol_table();
  // generate_memory_map();
  
  return 0;
}