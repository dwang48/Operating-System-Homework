#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <getopt.h>
#include <iomanip>
#include <cstring>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <sstream>
#include <limits.h>


using namespace std;

class Scheduler{
    

};



class FIFO: public Scheduler{

};

class SSTF: public Scheduler{

};

class LOOK: public Scheduler{

};

class CLOOK:public Scheduler{

};


class FLOOK: public Scheduler{

};


void simulation(){

}


int main(int argc, char* argv[]){
    //read input
    bool flag_v = false;
    bool flag_q = false;
    bool flag_f = false;
    string sched = "i";



    int opt;
    while((opt = getopt(argc, argv, "s:vqf")) != -1){
        switch(opt){
            case 's':
                sched = optarg;
                break;
            case 'v':
                flag_v = true;
                break;
            case 'q':
                flag_q = true;
                break;
            case 'f':
                flag_f = true;
                break;
            default:
                break;
        }
    }

    char* input = argv[optind];
    Scheduler* s = nullptr;
    switch(sched[0]){
        case 'i':
			s = new FIFO();
			break;
		case 'j':
			s = new SSTF();
			break;
		case 's':
			s = new LOOK();
			break;
		case 'c':
			s = new CLOOK();
			break;
		case 'f':
			s = new FLOOK();
			break;
		default:
			s = new FIFO();
			break;
    }
    
    ifstream in(input);
    if(!in.is_open()){
        exit(EXIT_FAILURE);
    }
    string line;
    while(getline(in,line)){
        if(in.eof()){
            break;
        }
        if(line[0] == '#' || !line.size()) {
			continue;
		}
        vector<int> res;
        istringstream iss(line);
        for(string s;iss >> s;){
            res.push_back(stoi(s));
        }
    }

    simulation();



    return 0;



}