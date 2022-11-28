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



using namespace std;

#define PTE 64


// #define 

// #define 
class myrandom{
    private:
        vector<int> randvals;
        int ofs = 0;
        int n;
    public:
        int get(int burst){
            int ans = 1 + (randvals[ofs++%n] % burst);
            return ans; 
        };
        myrandom(){};
        ~myrandom(){
            randvals.clear();
        }

        //for reading randomfile input
        myrandom(char* file){
            ifstream in(file);
            string size;
            getline(in,size);
            randvals.resize(stoi(size));
            n = randvals.size();
            int val;
            for(int i = 0; i < n; i++){
                in>>val;
                randvals[i] = val;
            }
        }
};




vector<string> split(){

}

struct vma{
    int start_vpage;
    int end_vpage;
    int write_protected;
    int file_mapped;

    vma(int s, int e, int w, int f){
        start_vpage = s;
        end_vpage = e;
        write_protected = w;
        file_mapped = f;
    }
};

struct pte_t{
    unsigned present:1;
    unsigned write_protected:1;
    unsigned modified:1;
    unsigned referenced:1;
    unsigned pageout:1;
    unsigned frame_index:7;
    //optinal
    unsigned file_mapped:1;
    unsigned page_index:6;
    unsigned just_referenced:1;
    unsigned not_assigned:12;
    
    pte_t(){
        present = 0;
        write_protected = 0;
        modified = 0;
        referenced = 0;
        pageout = 0;
        frame_index = 0;
        file_mapped = 0;
        page_index = 0;
        just_referenced = 0;
        not_assigned = 0;
    }
};

struct pstats_t{
    unsigned unmaps, maps, ins, outs, fins, fouts, zeros, segv, segprot;
    pstats_t(){
        unmaps = 0;
        maps = 0;
        ins = 0;
        outs = 0;
        fins = 0;
        fouts = 0;
        zeros = 0;
        segv = 0;
        segprot = 0;
    }
};


struct frame_t{
    unsigned frame_index:7;
    int pid;
    int vpage;
    int mapped;
    unsigned age;
    int tau;

    frame_t(unsigned index){
        frame_index = index;
        pid = 0;
        vpage = 0;
        mapped = 0;
        age = 0;
        tau = 0;
    }
};

class Pager{
    private:
        int frames;
        int hand;
    public:
        virtual frame_t* select_victim_frame() = 0;
        Pager(int f){
            frames = f;
        }
};

class FIFO : public Pager{
    private:

    public:

        FIFO(int frames) : Pager(frames){
        }
        frame_t* select_victim_frame(){
            //frame_t* frame_table
        }
};

class Random : public Pager{
    private:
        myrandom* rand;
    public:
        Random(int frames, char* randomfile) : Pager(frames){
            rand = new myrandom(randomfile);
        }

        frame_t* select_victim_frame(){

        }
};

class Clock : public Pager{

};

class NRU : public Pager{

};

class Aging : public Pager{

};

class Working_Set : public Pager{

};







void simulation(Pager* p){

}

int main(int argc, char* argv[]){
    int frames = 16;
    string algorithm = "f";
    string options = "OPFS";

//read input
int opt;
while((opt = getopt(argc, argv, "f:a:o:")) != -1){
    switch(opt){
        case 'f':
            frames = atoi(optarg);
        case 'a':
            options = optarg;
        case 'o':
            algorithm = optarg;
        default:
            break;
    }
}
char* inputfile = argv[optind];
char* randomfile = argv[optind+1];
Pager* p = nullptr;
switch(algorithm[0]){
    case 'f':
			p = new FIFO(frames);
			break;
		case 'r':
			p = new Random(frames, randomfile);
			break;
// 		case 'c':
// 			p = new Clock(frames);
// 			break;
// 		case 'e':
// 			p = new NRU(frames);
// 			break;
// 		case 'a':
// 			p = new Aging(frames);
// 			break;
// 		case 'w':
// 			p = new Working_Set(frames);
// 			break;
// 		default:
// 			p = new FIFO(frames);
// 			break;
}

bool option_O = false, option_P = false, option_F = false, option_S = false,
option_x = false, option_y = false, option_f = false, option_a = false;

if(options.find("O") != string::npos) option_O = true;
if(options.find("P") != string::npos) option_P = true;
if(options.find("F") != string::npos) option_F = true;
if(options.find("S") != string::npos) option_S = true;
if(options.find("x") != string::npos) option_x = true;
if(options.find("y") != string::npos) option_y = true;
if(options.find("f") != string::npos) option_f = true;
if(options.find("a") != string::npos) option_a = true;



ifstream in(inputfile);    
if(!in.is_open()){
    exit(EXIT_FAILURE);
}

string line = "";
while(!in.eof()){
    getline(in,line);
    if(line.size() > 0 && line[0] != '#'){
        break;
    }
}
int pnum = stoi(line);
for(int i = 0; i < pnum; i++){
    process
}



simulation();


//print outputs


// printf("PROC[%d]: U=%lu M=%lu I=%lu O=%lu FI=%lu FO=%lu Z=%lu SV=%lu SP=%lu\n",
//     proc->pid,
//     pstats->unmaps, pstats->maps, pstats->ins, pstats->outs,
//     pstats->fins, pstats->fouts, pstats->zeros,
//     pstats->segv, pstats->segprot);


// printf("TOTALCOST %lu %lu %lu %llu %lu\n",
// inst_count, ctx_switches, process_exits, cost, sizeof(pte_t));

return 0;

}