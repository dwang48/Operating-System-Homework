#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>
#include <map>
#include <algorithm>
#include <stdio.h>
#include <string.h>

using namespace std;


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
//4 instructions
void relative();
void external();
void immediate(string input, int idx);
void absolute();







void read(char* argv[]){
  string line;
  ifstream input(argv[1]);
  ifstream output(argv[2]);
  if(input.is_open()){
    while(getline(input,line)){
      cout<<line<<endl;
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



void immediate(string input, int idx){
  if(input.size()>4){
    cout<<" Error: Illegal immediate value; treated as 9999" << endl;
  }
  else{
    int address = 
  }
}



int main (int argc, char* argv[])
{
  cout<<argc<<endl;
  if(argc !=3){
    cout<< "Both Input file and Out file required."<<endl;
    exit(EXIT_FAILURE);
  }
  read(argv);
  return 0;
}