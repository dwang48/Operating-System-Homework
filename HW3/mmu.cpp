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

//define cost of operations
#define PTE 64
#define READ 1
#define WRITE 1
#define CONTEXT_SWITCHES 130
#define PROCESS_EXITS 1250

//optional operations
#define MAPS 300
#define UNMAPS 400
#define INS 3100
#define OUTS 2700
#define FINS 2800
#define FOUTS 2400
#define ZEROS 140
#define SEGV 340
#define SEGPROT 420

unsigned long context_switches = 0;
unsigned long process_exits = 0;
int instr_idx = 0;


class myrandom{
    private:
        vector<int> randvals;
        int ofs = 0;
        int n;
    public:
        int get(int burst){
            int ans = (randvals[ofs++%n] % burst);
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




// vector<string> split(){

// }

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
    unsigned long unmaps, maps, ins, outs, fins, fouts, zeros, segv, segprot;
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

struct process{
    int pid;
    vector<vma*> vma_vec;
    pte_t* page_table[PTE];
    pstats_t pstats;

    process(int id){
        pid = id;
        for(int i = 0; i < PTE; i++){
            pte_t* temp = new pte_t;
            page_table[i] = temp;
        }
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

struct instr{
    string op;
    int vpage;
    instr(string o, int v){
        op = o;
        vpage = v;
    }
};

vector<process*> plist;
vector<instr> ilist;



class Pager{
    private:
    public:
        int frames;
        int handle;
        vector<frame_t*> frame_table;
        list<frame_t*> free_table;
        virtual frame_t* select_victim_frame() = 0;
        frame_t* allocate(){
            if(!free_table.empty()){
                frame_t* frame = free_table.front();
                free_table.pop_front();
                return frame;
            }
            else{
                return nullptr;
            }
        }
        frame_t* get(){
            frame_t *cur = allocate();
            if(!cur){
                cur = select_victim_frame();
            }
            return cur;
        }
        Pager(){
        }
        Pager(int f){
            frames = f;
            for(int i = 0; i < frames; i++){
                frame_t* temp = new frame_t(i);
                frame_table.emplace_back(temp);
                free_table.emplace_back(temp);
            }
            handle = 0;
        }
        void print(){
            printf("FT:");
            for(int i = 0; i < frame_table.size();i++){
                if(frame_table[i]->mapped){
                    cout<<" "<<frame_table[i]->pid<<":"<<frame_table[i]->vpage;
                }
                else{
                    cout<< " *";
                }
            }
            printf("\n");
        }
};

class FIFO : public Pager{
    private:

    public:
        FIFO(int f) : Pager(f){
        };
        frame_t* select_victim_frame(){
            frame_t* frame = frame_table[handle++%frames];
            return frame;
        }
};

class Random : public Pager{
    private:
        myrandom* rand;
    public:
        Random(int f, char* randomfile) : Pager(f){
            rand = new myrandom(randomfile);
        }

        frame_t* select_victim_frame(){
            int r = rand->get(frames);
            //cout<<"random:"<<r<<endl;
            frame_t* frame = frame_table[r];
            return frame;

        }
};

class Clock : public Pager{
    public:
    Clock(int f) : Pager(f){
        };
      frame_t* select_victim_frame(){
            frame_t* frame = frame_table[handle++%frames];
            return frame;
        }
};

class NRU : public Pager{
    public:
    NRU(int f) : Pager(f){
        };
    frame_t* select_victim_frame(){
            frame_t* frame = frame_table[handle++%frames];
            return frame;
        }
};

class Aging : public Pager{
    public:
    Aging(int f) : Pager(f){
        };
    frame_t* select_victim_frame(){
            frame_t* frame = frame_table[handle++%frames];
            return frame;
        }
};

class Working_Set : public Pager{
    public:
    Working_Set(int f) : Pager(f){
        };
    frame_t* select_victim_frame(){
            frame_t* frame = frame_table[handle++%frames];
            return frame;
        }
};









void simulation(Pager* p, bool option_O,bool option_x){
    process* cur = nullptr;

    for(int i = 0; i < ilist.size();i++){
        string op = ilist[i].op;
        int vpage = ilist[i].vpage;

        if(option_O){
            cout<<i<<": ==> " << op<<" "<< vpage<<endl;
        }
        // handle special case of “c” and “e” instruction
        if(op == "c"){
            context_switches++;
            cur = plist[vpage];
        }
        else if(op == "e"){
            if(option_O){
                cout << "EXIT current process " << cur->pid<<endl;
            }
            for(int j = 0; j < PTE; j++){
                if(cur->page_table[j]->present){
                    if(option_O){
                        cout<<" UNMAP "<<cur->pid<<":"<<j<<endl;
                    }
                    cur->pstats.unmaps++;
                    if(cur->page_table[j]->modified && cur->page_table[j]->file_mapped){
                        if(option_O){
                            cout<< " FOUT"<< endl;
                        }
                        cur->pstats.fouts++;
                    }
                    int idx = cur->page_table[j]->frame_index;
                    p->frame_table[idx]->pid = 0;
                    p->frame_table[idx]->vpage = 0;
                    p->frame_table[idx]->mapped = 0;
                    p->frame_table[idx]->age = 0;
                    p->frame_table[idx]->tau = 0;
                    p->free_table.emplace_back(p->frame_table[idx]);
                }
                cur->page_table[j]->present = 0;
                cur->page_table[j]->write_protected = 0;
                cur->page_table[j]->modified = 0;
                cur->page_table[j]->pageout = 0;
                cur->page_table[j]->frame_index = 0;
                cur->page_table[j]->file_mapped = 0;
            }
            process_exits++;
            cur = nullptr;
        }
        // now the real instructions for read and write
        else{
            pte_t *pte = cur->page_table[vpage];
            if(!pte->present){
                // this in reality generates the page fault exception and now you execute
                // verify this is actually a valid page in a vma if not raise error and next inst
                bool flag = true;
                for(int i = 0; i < cur->vma_vec.size();i++){
                    if(cur->vma_vec[i]->start_vpage<=vpage && vpage <= cur->vma_vec[i]->end_vpage){
                        pte->write_protected = cur->vma_vec[i]->write_protected;
                        pte->file_mapped = cur->vma_vec[i]->file_mapped;
                        flag = false;
                        break;
                    }
                }
                if(flag){
                    if(option_O){
                        cout<<" SEGV"<<endl;
                    }
                    cur->pstats.segv++;
                    continue;
                }
                frame_t* newframe = p->get();
                //-> figure out if/what to do with old frame if it was mapped
                // see general outline in MM-slides under Lab3 header and writeup below
                // see whether and how to bring in the content of the access page.
                if(newframe->mapped){
                    if(option_O){
                        cout << " UNMAP " << newframe->pid<<":"<<newframe->vpage<<endl;
                    }
                    plist[newframe->pid]->page_table[newframe->vpage]->present = 0;
                    plist[newframe->pid]->page_table[newframe->vpage]->referenced = 0;
                    plist[newframe->pid]->pstats.unmaps++;
                    if(plist[newframe->pid]->page_table[newframe->vpage]->modified){
                        if(plist[newframe->pid]->page_table[newframe->vpage]->file_mapped){
                            if(option_O){
                                cout<<" FOUT"<<endl;
                            }
                            plist[newframe->pid]->pstats.fouts++;
                        }
                        else{
                            if(option_O){
                                cout<< " OUT"<<endl;
                            }
                            plist[newframe->pid]->page_table[newframe->vpage]->pageout = 1;
                            plist[newframe->pid]->pstats.outs++;
                        }
                        plist[newframe->pid]->page_table[newframe->vpage]->modified = 0;
                    }
                }
                if(pte->file_mapped){
                    if(option_O){
                        cout<<" FIN"<<endl;
                    }
                    cur->pstats.fins++;
                }
                else if(pte->pageout){
                    if(option_O){
                        cout<<" IN"<<endl;
                    }
                    cur->pstats.ins++;
                }
                else{
                    if(option_O){
                        cout<< " ZERO"<<endl;
                    }
                    cur->pstats.zeros++;
                }
                if(option_O){
                    cout << " MAP " << newframe->frame_index<<endl;
                }
                pte->present = 1;
                pte->frame_index = newframe->frame_index;
                newframe->pid = cur->pid;
                newframe->vpage = vpage;
                newframe->mapped = 1;
                newframe->age = 0;
                newframe->tau = instr_idx;
                cur->pstats.maps++;                
            }
            pte->referenced = 1;
            if(op == "w"){
                if(pte->write_protected){
                    if(option_O){
                        cout<<" SEGPROT"<<endl;
                    }
                    cur->pstats.segprot++;
                }
                else{
                    pte->modified = 1;
                }
            }
            if(option_x){
                cout << "PT[" << cur->pid << "]:";
                for(int j = 0; j < PTE; j++) {
                    //cout << page_table[i]->VALID << " " << page_table[i]->WRITE_PROTECTED << " " << page_table[i]->MODIFIED << " " << page_table[i]->REFERENCED << " " << page_table[i]->PAGEDOUT << " " << page_table[i]->FRAME_INDEX << "\n";
                    if(cur->page_table[j]->present) {
                        string ref = cur->page_table[j]->referenced? "R" : "-";
                        string mod = cur->page_table[j]->modified? "M" : "-";
                        string pag = cur->page_table[j]->pageout? "S" : "-";
                        cout << " " << cur << ":" << ref << mod << pag;
                    }
                    else {
                        cout << " " << (cur->page_table[j]->pageout? "#" : "*");
                    }
                }
                cout << endl;
            }

        // now the page is definitely present
        // check write protection
        // simulate instruction execution by hardware by updating the R/M PTE bits
        }
    }

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
                break;
            case 'o':
                options = optarg;
                break;
            case 'a':
                algorithm = optarg;
                break;
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
            case 'c':
                p = new Clock(frames);
                break;
            case 'e':
                p = new NRU(frames);
                break;
            case 'a':
                p = new Aging(frames);
                break;
            case 'w':
                p = new Working_Set(frames);
                break;
            // default:
            //     p = new FIFO(frames);
            //     break;
    }
    //cout<<algorithm[0]<<endl;

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

    //cout<<options<<endl;

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
        process* temp = new process(i);
        plist.push_back(temp);
        while(getline(in,line)){
            if(line.size() > 0 && line[0] != '#'){
                break;
            }
        }
        int num = stoi(line);
        for(int j = 0;j < num;j++){
            while(getline(in,line)){
                if(line.size() > 0 && line[0] != '#'){
                    break;
                }
            }
            vector<string> res;
            string p = " ";
            string::size_type d;
            line += p;
            int i = 0;
            while(i < line.size()) {
                d = line.find(p, i);
                if(d < line.size()) {
                    res.push_back(line.substr(i, d - i));
                    i = d + p.size();
                }
                else {
                    i++;
                }
            }
            vma* new_vma = new vma(stoi(res[0]),stoi(res[1]),stoi(res[2]),stoi(res[3]));
            temp->vma_vec.push_back(new_vma);
        }
    }
    
    while(getline(in,line)){
        if(in.eof()){
            break;
        }
        if(line[0] == '#' || line.length() == 0) {
			continue;
		}
        vector<string> res;
        string p = " ";
        string::size_type d;
        line += p;
        int i = 0;
        while(i < line.size()) {
            d = line.find(p, i);
            if(d < line.size()) {
                res.push_back(line.substr(i, d - i));
                i = d + p.size();
            }
            else {
                i++;
            }
        }
        instr new_instr = instr(res[0],stoi(res[1]));
        ilist.emplace_back(new_instr);
    }
    //check read in
    // cout<<"ilist: " << ilist.size()<<endl;
    // cout<<"plist: " << plist.size()<<endl;

    simulation(p,option_O,option_x);


    //print outputs
    if(option_P) {
        for(int i = 0; i < plist.size(); i++) {
            cout << "PT[" << plist[i]->pid << "]:";
            for(int j = 0; j < PTE; j++) {
                if(plist[i]->page_table[j]->present) {
                    string ref = plist[i]->page_table[j]->referenced? "R" : "-";
                    string mod = plist[i]->page_table[j]->modified? "M" : "-";
                    string pag = plist[i]->page_table[j]->pageout? "S" : "-";
                    cout << " " << j << ":" << ref << mod << pag;
                }
                else {
                    cout << " " << (plist[i]->page_table[j]->pageout? "#" : "*");
                }
            }
            cout << endl;
        }
    }

    // FT
    if(option_F) {
        p->print();
    }


    if(option_S) {
        unsigned long long cost = 0;
        // PROC
        for(int i = 0; i < plist.size(); i++) {
            pstats_t pstats = plist[i]->pstats;
            printf("PROC[%d]: U=%lu M=%lu I=%lu O=%lu FI=%lu FO=%lu Z=%lu SV=%lu SP=%lu\n",
                plist[i]->pid,
                pstats.unmaps,
                pstats.maps,
                pstats.ins,
                pstats.outs,
                pstats.fins,
                pstats.fouts,
                pstats.zeros,
                pstats.segv,
                pstats.segprot);
                cost += pstats.unmaps * UNMAPS + pstats.maps *MAPS +
                pstats.ins * INS + pstats.outs * OUTS +
                pstats.fins * FINS+ pstats.fouts * FOUTS +
                pstats.zeros * ZEROS +
                pstats.segv * SEGV +
                pstats.segprot * SEGPROT;
        }
        // TOTALCOST
        cost += ilist.size() + context_switches * CONTEXT_SWITCHES + process_exits * PROCESS_EXITS-context_switches-process_exits;
        printf("TOTALCOST %lu %lu %lu %llu %lu\n", ilist.size(), context_switches, process_exits, cost,sizeof(pte_t));
    }


    return 0;

}