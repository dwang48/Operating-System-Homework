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
#include<iomanip>

using namespace std;

struct Symbol{
  string name;
  int errcode = -1;
  int val;
  bool isUsed = false;
  int modulenum;
  int moduleoffset;

};

struct Memory{
  string name;
  //int warning = -1;
  int op;
  int errcode = -1;
  int address;
};

ifstream file;
int module;
int linenum;
int lineoffset;
int baseaddr;
int total_instr;

string line;

char* token;
char* buffer;
//symbol table
vector<Symbol> symbols;

//hashmap and hashset for quick lookup
unordered_set<string> instructions = {"R","E","I","A"};
//unordered_map<string> used_symbols;


char* getToken(){
  if(token!=NULL){
    token = strtok(NULL," \t\n");
    if(token!=NULL){
      lineoffset = token-buffer+1;
      return token;
    }
    else{
      return getToken();
    }
  }
  else{
    if(!file.eof()){
      string linecopy = line;
      getline(file,line);
      if(file.eof()){
        lineoffset = linecopy.size()+1;
        return NULL;
      }
      delete[] buffer;
      buffer = new char[line.size()+1];
      strcpy(buffer,line.c_str());
      linenum++;
      token = strtok(buffer," \t\n");
      if(token==NULL){
        lineoffset = line.size()+1;
        return getToken();
      }
      else{
        lineoffset = token-buffer+1;
        return token;
      }
    }
  }
} 

void __parseerror(int errcode,int linenum, int lineoffset) { 
    if(errcode == -1){
      cout<<"No Error!"<<endl;
    }
    else{
      vector<string> errors = { 
        "NUM_EXPECTED",        //0 Number expect, anything >= 2^30 is not a number either 
        "SYM_EXPECTED",   //1 Symbol Expected 
        "ADDR_EXPECTED",  //2 Addressing Expected which is A/E/I/R   
        "SYM_TOO_LONG",  //3 Symbol Name is too long 
        "TOO_MANY_DEF_IN_MODULE", //4  > 16  
        "TOO_MANY_USE_IN_MODULE",     //5  > 16 
        "TOO_MANY_INSTR"             //6  total num_instr exceeds memory size (512)   
    }; 
      cout<<"Parse Error line "<<linenum<< " offset "<< lineoffset <<": "<<errors[errcode]<<endl; 
    }
    
}

void __errors(int errcode, string symbols_name){
  switch(errcode){
    case 2:
			cout<<"Error: This variable is multiple times defined; first value used"<<endl;
			break;
		case 3:
			cout<<"Error: "<< symbols_name<<" is not defined; zero used"<<endl;
			break;
		case 6:
			cout<<"Error: External address exceeds length of uselist; treated as immediate"<<endl;
			break;
		case 8:
			cout<<"Error: Absolute address exceeds machine size; zero used"<<endl;
			break;
		case 9:
			cout<<"Error: Relative address exceeds module size; zero used"<<endl;
			break;
		case 10:
			cout<<"Error: Illegal immediate value; treated as 9999"<<endl;
			break;
		case 11:
			cout<<"Error: Illegal opcode; treated as 9999"<<endl;
			break;
		// default:
		// 	cout<<"Not a valid error code"<<endl;
		// 	break;
  }
}

void __warnings(int warning_code, int module_num, int module_offset, string name, int size){
  switch(warning_code){
		case 0:
			cout << "Warning: Module " << module_num << ": " << name << " too big " << module_offset << " (max=" << size << ") assume zero relative"<<endl;
			break;
		case 1:															
			cout << "Warning: Module " << module_num << ": " << name << " appeared in the uselist but was not actually used"<<endl;
			break;
		case 2:						
			cout << "Warning: Module " << module_num << ": " << name << " was defined but never used"<<endl;
			break;
		default:
			cout<<"Not a valid warning code"<<endl;
			break;
	}
}


bool isSymbol(char* input){
  string s = input;
  regex rule("[a-zA-Z][a-zA-Z0-9]*");
  return regex_match(s.begin(),s.end(),rule);
}

Symbol readSymbol(){
  char* input = getToken();
  Symbol s;
  
  if(input){
    if(isSymbol(input)){
      string stoken = input;
      if(stoken.size()>16){
        __parseerror(3,linenum,lineoffset);
        exit(EXIT_FAILURE);
      }
      else{
        s.name = stoken;
        s.modulenum = module;
        return s;
      }
    }
    else{
      __parseerror(1,linenum,lineoffset);
      exit(EXIT_FAILURE);
    }
  }
  else{
    __parseerror(1,linenum,lineoffset);
    exit(EXIT_FAILURE);
  }
}


bool isInt(char* input){
  string s = input;
  regex rule("[0-9]+");
  return regex_match(s.begin(),s.end(),rule);
}

int readInt(string s){
  char* input = getToken();
  int linenum_copy = linenum;
  if(input==NULL){
    if(s=="defcount"){
      return -1;
    }
    else{
      __parseerror(0,linenum,lineoffset);
      exit(EXIT_FAILURE);
    }
  }
  else{
    if(isInt(input)){
      return atoi(input);
    }
    else{
      __parseerror(0,linenum,lineoffset);
      exit(EXIT_FAILURE);
    }
  }
}

bool isIAER(char* input){
  string s = input;
  if(instructions.find(s)!=instructions.end()){
    return true;
  }
  else{
    return false;
  }
}

string readIAER(){
  char* input = getToken();
  if(input){
    if(isIAER(input)){
      string stoken = token;
      return stoken;
    }
    else{
      __parseerror(2,linenum,lineoffset);
      exit(EXIT_FAILURE);
    }
  }
  else{
    __parseerror(2,linenum,lineoffset);
    exit(EXIT_FAILURE);
  }
}

void createSymbol(Symbol sym, int val){
  for(int i = 0; i < symbols.size();i++){
    if(symbols[i].name==sym.name){
      symbols[i].errcode = sym.errcode = 2;
      break;
    }
  }
  if(sym.errcode!=2){
    sym.moduleoffset = val;
    sym.val = val+baseaddr;
    symbols.push_back(sym);
  }
}



void generate_symbol_table(){
  cout<<"Symbol Table"<<endl;
  for(int i = 0; i < symbols.size();i++){
    cout<<symbols[i].name<<"="<<symbols[i].val;
    if(symbols[i].errcode ==2){
      cout<<" ";
      __errors(2,"");
      symbols[i].errcode = -1;
    }
    else{
      cout<<endl;
    }
  }
  cout<<endl;
}




void pass1(char* argv[]){
  module  = 0;
  linenum = 0;
  lineoffset = 0;
  baseaddr = 0;
  total_instr = 0;

  buffer = NULL;
  token = NULL;


  file.clear();
  file.open(argv[1]);

  if(!file.is_open()){
    cout<<"Cannot open file"<<endl;
    exit(EXIT_FAILURE);
  }


  while(!file.eof()){
    module++;
    //cout<<module;
    // **********/deflist/**************
    int defcount = readInt("defcount");
    //cout<<defcount;
    if(file.eof()){
      break;
    }
    if(defcount == -1 && !file.eof()){
      __parseerror(0,linenum,lineoffset);
      exit(EXIT_FAILURE);
    }
    else if(defcount > 16){
      __parseerror(4,linenum,lineoffset); //TOO_MANY_DEF_IN_MODULE
      exit(EXIT_FAILURE);
    }
    for(int i = 0; i < defcount;i++){
      Symbol sym = readSymbol();
      int val = readInt("");
      createSymbol(sym,val);
    }

    //***************/uselist/************************
    int usecount = readInt("");
    //cout<<usecount<<endl;
    if(usecount>16){
      __parseerror(5,linenum,lineoffset); //TOO_MANY_USE_IN_MODULE
      exit(EXIT_FAILURE);
    }
    for(int i = 0; i < usecount; i++){
      readSymbol();
    }


    //***************//instrlist/*********************
    int num_instr = readInt("");
    total_instr+= num_instr;
    //cout<<num_instr<<endl;
    if(total_instr>512){
      __parseerror(6,linenum,lineoffset);
      exit(EXIT_FAILURE);
    }
    for(int i = 0; i < num_instr;i++){
      readIAER();
      readInt("");
    }

    for(int i = 0;i < symbols.size();i++){
      if(symbols[i].modulenum==module && symbols[i].moduleoffset>num_instr){
        __warnings(0,module,symbols[i].moduleoffset,symbols[i].name,num_instr-1);
        symbols[i].moduleoffset = 0;
        symbols[i].val = baseaddr;
      }
    }

    baseaddr+=num_instr;
    //cout<<baseaddr<<endl;
  }
  
  generate_symbol_table();
  file.close();
}

void relative(int op,vector<Memory>& memmap, int num_instr, int i){
  Memory m;
  m.address = baseaddr+i;
  if(op>9999){
    m.op = 9999;
    m.errcode = 11;
    memmap.push_back(m);
    return;
  }
  int opcode = op/1000;
  int operand = op%1000;
  if(operand > num_instr){
    m.op = opcode*1000+baseaddr;
    m.errcode = 9;
    memmap.push_back(m);
    return;
  }
  m.op = op + baseaddr;
  memmap.push_back(m);
}

void external(int op, vector<Memory>& Memmap,int usecount,vector<Symbol>& use_list,int i){
  Memory m;
  m.address = baseaddr+i;
  if(op>9999){
    m.op = 9999;
    m.errcode = 11;
    Memmap.push_back(m);
    return;
  }
  int opcode = op/1000;
  int operand = op%1000;
  if(operand>=usecount){
    m.op = op;
    m.errcode = 6;
    Memmap.push_back(m);
    return;
  }

  m.op = use_list[operand].val+opcode*1000;
  // cout<<"operand:"<<operand<<endl;
  // cout<<"val:"<<use_list[operand].name<<endl;
  // cout<<m.op<<endl;
  use_list[operand].isUsed = true;
  if(use_list[operand].errcode!=2){
    m.errcode = use_list[operand].errcode;
  }
  else{
    m.errcode = -1;
  }
  
  if(m.errcode==3){
    m.name = use_list[operand].name;
    m.op = m.op/1000*1000;
  }
  Memmap.push_back(m);
}


void immediate(int op, vector<Memory>& Memmap,int i){
  Memory m;
  m.address = baseaddr+i;
  if(op>9999){
    m.op = 9999;
    m.errcode = 10;
    Memmap.push_back(m);
    return;
  }
  else{
    m.op = op;
    Memmap.push_back(m);
  }
}


void absolute(int op, vector<Memory>& memmap, int i){
  Memory m;
  m.address = baseaddr+i;
  if(op>9999){
    m.op = 9999;
    m.errcode = 11;
    memmap.push_back(m);
    return;
  }
  int opcode = op/1000;
  int operand = op%1000;
  if(operand>=512){
    m.errcode = 8;
    m.op = opcode*1000;
    memmap.push_back(m);
    return;
  }
  m.op = op;
  memmap.push_back(m);
}



void generate_memory_map(vector<Memory>& memmap,vector<Symbol>& use_list,int module){
  for(int i = 0; i < memmap.size(); i++){
    if(memmap[i].errcode==-1){
          //cout<<memmap[i].address;
          cout<<setw(3)<<setfill('0')<<memmap[i].address<<": ";
          cout<<setw(4)<<setfill('0')<<memmap[i].op<<endl;
    }
    else{
          cout<<setw(3)<<setfill('0')<<memmap[i].address<<": ";
          cout<<setw(4)<<setfill('0')<<memmap[i].op<<" ";
          __errors(memmap[i].errcode,memmap[i].name);
    }
  }
  for(int i = 0; i < use_list.size();i++){
      if(use_list[i].isUsed==false){
        __warnings(1,module,0,use_list[i].name,0);
      }
  }
}

void pass2(char* argv[]){
  cout<<"Memory Map"<<endl;
  baseaddr = 0;
  module  = 0;
  linenum = 0;
  lineoffset = 0;
  total_instr = 0;

  buffer = NULL;
  token = NULL;


  file.clear();
  file.open(argv[1]);

  while(!file.eof()){
    vector<Memory> memmap;
    module++;
    //cout<<module;
    // **********/deflist/**************
    int defcount = readInt("defcount");
    //cout<<defcount;
    if(file.eof()){
      break;
    }

    for(int i = 0; i < defcount;i++){
      readSymbol();
      readInt("");
    }

    //***************/uselist/*******************
    int usecount = readInt("");
    vector<Symbol> use_list;
    for(int i = 0; i < usecount; i++){
      Symbol sym = readSymbol();
      bool flag = true;
      for(int j = 0; j < symbols.size();j++){
        if(sym.name==symbols[j].name){
          use_list.emplace_back(symbols[j]);
          symbols[j].isUsed = true;
          flag = false;
          break;
        }
      }
      if(flag){
        sym.errcode = 3;
        createSymbol(sym,0);
        for(int j = 0; j < symbols.size();j++){
          if(sym.name==symbols[j].name){
            use_list.emplace_back(symbols[j]);
            symbols[j].isUsed = true;
            break;
          }
      }
      }
    }    



    //******************/instruction list/**********************
    int num_instr = readInt("");
    total_instr+= num_instr;
    for(int i = 0; i < num_instr;i++){
      string instr = readIAER();
      int op = readInt("");
      if(instr=="I"){
        immediate(op,memmap,i);
      }
      else if(instr=="A"){
        absolute(op,memmap,i);
      }
      else if(instr=="E"){
        external(op,memmap,usecount,use_list,i);
      }
      else if(instr=="R"){
        relative(op,memmap,num_instr,i);
      }
    }
    baseaddr+=num_instr;
    generate_memory_map(memmap,use_list,module); 
  }
  cout<<endl;
  
  for(int i = 0;i < symbols.size();i++){
    if(symbols[i].isUsed==false){
      __warnings(2,symbols[i].modulenum,0,symbols[i].name,0);
    }
  }

  cout<<endl;
  file.close();
}


int main (int argc, char* argv[])
{
  if(argc !=2){
    cout<< "One input file is required."<<endl;
    exit(EXIT_FAILURE);
  }
  pass1(argv);
  pass2(argv);
}