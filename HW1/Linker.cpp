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
vector<vector<string>> tokens_list;
vector<string> tokens;
vector<int> rows;
vector<int> cols;


unordered_set<char> instructions = {'R','E','I','A'};



//For print parse errors
int linenum,lineoffset;
int current_token = 0;
int total_tokens;



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
vector<vector<string>> tokens_parser(vector<string>& tokens);
string definition_parser(string input);
string use_parser(string input);
string program_parser(string input);

//4 instructions
void relative(string input, int );
void external();
void immediate(string input, int idx);
void absolute();


void read(char* argv[]){
  string line;
  ifstream input(argv[1]);
  ifstream output(argv[2]);
  if(input.is_open()){
    int row = 0, col = -1;
    while(getline(input,line,'\n')){
      row++;
      tokens.push_back(line);
      rows.push_back(row);
      cols.push_back(++col);
    }
  input.close();
  }
  else{
    cout<<"Unable to open file";
  }
}

int converter(string input){
  if(input.size()>4){
    return 9999;
  }
  else{
    return stoi(input);
  }
}

vector<vector<string>> tokens_parser(vector<string>& tokens){
  int n = tokens.size();
  tokens_list.resize(n);
  for(int i = 0; i < n; i++){
    istringstream iss(tokens[i]);
    string current;
    while(getline(iss,current,' ')){
      tokens_list[i].push_back(current);
    }
  }
  return tokens_list;
}

void relative(string input){

}


void immediate(string input, int idx){
  if(input.size()>4){
    cout<<" Error: Illegal immediate value; treated as 9999" << endl;
  }
  else{
    int address = stoi(input);
  }
}


void absolute(){

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





int main (int argc, char* argv[])
{
  if(argc !=3){
    cout<< "Both Input file and Out file required."<<endl;
    exit(EXIT_FAILURE);
  }
  read(argv);
  tokens_list = tokens_parser(tokens);

  // for(int i = 0; i < tokens_list.size();i++){
  //   for(int j = 0; j < tokens_list[i].size();j++){
  //     cout<<tokens_list[i][j]<<"   "<< i << j << endl;
  //   }
  // }
  
  
  //first pass
    
  
  //second pass
  
  
  return 0;
}