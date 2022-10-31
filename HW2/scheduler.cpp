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

using namespace std;


typedef enum{
CREATED,
READY,
RUNNING,
BLOCKED,
FINISHED
} process_state_t;

typedef enum{
    TRANS_TO_READY,
    TRANS_TO_PREEMPT,
    TRANS_TO_RUN,
    TRANS_TO_BLOCK
} trans_state_t;


class myrandom{
    private:
        vector<int> randvals;
        int ofs;
    public:
        int get(int burst){
            return 1 + (randvals[ofs++] % burst); 
        };
        myrandom(){};

        //for reading randomfile input
        myrandom(char* file){
            ifstream in(file);
            if(!in.is_open()){
                exit(EXIT_FAILURE);
            }
            string size;
            getline(in,size);
            randvals.resize(stoi(size));
            int n = randvals.size();
            int val;
            for(int i = 0; i < n; i++){
                in>>val;
                randvals[i] = val;
            }
            ofs = 0;
        }

    



};



struct process{
    int pid; //process id


    int AT; // arrival time
    int TC; // total cpu time
    int CB; // CPU Burst
    int IO; // IO Burst
    int PRIO; //priority
    
    int FT; //finish time
    int TT; //Turnaround time( finishing time  -   AT )
    int IT; //IO time(time in blocked state)
    int CW; //CPU waiting time



    process_state_t state;
    int state_ts;//time stamp



    void init(int arrival_time, int total_cpu_time, int CPU_burst, int IO_burst){
        AT = arrival_time;
        TC = total_cpu_time;
        CB = CPU_burst;
        IO = IO_burst;
    };
};
struct event{
    int t; //time stamp
    process* p; //process
    process_state_t prev; // prev state
    process_state_t next; // next state
    trans_state_t tran; // transition

    event(int time, process *proc, process_state_t prev_state, process_state_t next_state, trans_state_t transition){
        t = time;
        p = proc;
        prev = prev_state;
        next = next_state;
        tran = transition;
    }    
};


struct event_list{
    list<event *> l;
    int get_next_event_time(){
        if(!l.empty()){
            return -1; 
        }
        else{
            return l.front()->t;
        }
    }
    event *get_event(){
        if(!l.empty()){
            return nullptr; 
        }
        else{
            event *ans = l.front();
            l.pop_front();
            return ans;
        }
    }
    void push(event *e){
        if(l.empty()){
            l.push_back(e);
        }
        else{
            list<event*>::iterator it;
            for(it = l.begin();it!=l.end();++it){
                if((*it)->t>e->t){
                    l.insert(it,e);
                    break;
                }
            }
            if(it==l.end()){
                l.push_back(e);
            }
        }
    }

    // show all the value in the list
    // void show(){
    //     for(list<event *>::iterator<event*> it = q.begin(); it != q.end(); it++){
    //         cout<<it->
    //     }
    // }

    


};

class scheduler{
public:
	scheduler() {};
	virtual ~scheduler() {};
    void add_process(process *p) {};
    process* get_next_process(){return nullptr;};
    scheduler_type type;


private:

};


//FCFS,  LCFS,  SRTF,  RR  (RoundRobin),  PRIO  (PriorityScheduler)  and  PREemptive  PRIO  (PREPRIO)
typedef enum{
    F,L,S,R,P,E
} scheduler_type;

class FCFS:public scheduler{
    private:
        list<process*> l;
    public:
        FCFS(){
            type = F;
        }
        void add_process(process* p){
            p->state = READY;
            l.push_back(p);
        }
        process* get_next_process(){
            if(l.empty()){
                return nullptr;
            }
            else{
                auto p = l.front();
                l.pop_front();
                return p;
            }
        }
};
class LCFS:public scheduler{
    private:
        list<process*> l;
    public:
    LCFS(){
        type = L;
    }
    void add_process(process* p){
            p->state = READY;
            l.push_back(p);
    }

    
};
class SRTF:public scheduler{
    private:
        list<process*> l;
    public:
    SRTF(){
        type = S;
    }
    void add_process(process* p){
            p->state = READY;
            
    }

};
class RR:public scheduler{
    private:
        list<process*> l;
    public:
    RR(int quantum){
        type = R;
    }
    
};
class PRIO:public scheduler{
private:
    list<process *> active;
	list<process *> expired;
public:
    PRIO(){
        type = P;
    }

};
class PREPRIO:public scheduler{
private:
    list<process *> active;
	list<process *> expired;

public:
    PREPRIO(){
        type = E;
    }
    

};

void print_event(){

}

void simulation(scheduler * sched, event_list * el, int quantum){
    event* evt;
    int current_time;
    bool call_scheduler = false;
    process * current_running_process = nullptr;
    while((evt = el->get_event())){
        process *p = evt->p;
        current_time = evt->t;
        int transition = evt->tran;
        int timeInPrevState = current_time - p->state_ts;

        switch(evt->tran){
            case TRANS_TO_READY:
                // must come from BLOCKED or CREATED  
                // add to run queue, no event created


                call_scheduler = true;
                break;



            case TRANS_TO_PREEMPT:
                // must come from RUNNING (preemption)  
                // add to runqueue (no event is generated)
                call_scheduler = true;
                break;

            case TRANS_TO_RUN:
                // create event for either preemption or blocking

                break;


            case TRANS_TO_BLOCK:
                // create an event for when process becomes READY again


                call_scheduler = true;    
                break;

        }

        delete evt;
        evt = nullptr;


        if(call_scheduler){
            if(el->get_next_event_time() == current_time){
                continue;
            }
            call_scheduler = false;
            if(current_running_process = nullptr){
                current_running_process = sched->get_next_process();
                if(current_running_process == nullptr){
                    continue;
                }
                event *evt2 = new event(current_time,current_running_process,READY,RUNNING,TRANS_TO_RUN);
                el->push(evt2);
            }
        }
    }
}








int main(int argc, char* argv[]){
    string mode = "";
    int maxprios = 4;
    int quantum = -1;
    bool preempt = false;
    vector<process*> plist; 
    int opt;
    while((opt = getopt(argc, argv, "vteps:")) != -1){
        switch(opt){
            case 'v':
                cout<<" sdasd"<<endl;
                break;
            case 't':
                cout<<" sdasd"<<endl;
                break;
            case 'e':
                break;
            case 'p':
                break;
            case 's':
                mode = optarg;
                //cout<<mode<<endl;
                break;
            default:
                break;
        }
    }

scheduler* sched = nullptr;

   switch(mode[0]){
    case 'F':
        sched = new FCFS();
        break;
    case 'L':
        sched = new LCFS();
        break;
    case 'S':
        sched = new SRTF();
        break;
    case 'R':
        quantum = stoi(mode.substr(1,mode.size()));
        cout<<quantum;
        sched = new RR(quantum);
        break;
    case 'P':
        sched = new PRIO();
        

        break;
    case 'E':
        break;
    default:
        break;
   
   }

    char *inputfile = argv[optind];
    char *randfile = argv[optind+1];

    myrandom rand(randfile);

    //cout<<inputfile;
    ifstream in(inputfile);
    if(!in.is_open()){
        cout<<"File cannot be opened!";
        exit(EXIT_FAILURE);
    }
    int AT,TC,CB,IO,PRIO;
    while(!in.eof()){
        in>>AT>>TC>>CB>>IO;
        if(in.eof()){
            break;
        }
        PRIO = rand.get(maxprios);


        //cout<<AT<<"  "<<TC<<"  "<<CB<<"  "<<IO<<endl;
    }
   
   
    // printf("%04d: %4d %4d %4d %4d %1d | %5d %5d %5d %5d\n",
    // printf(“SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n",       
    event_list* el = new event_list();


    simulation(sched,el,quantum);
    cout<<mode<<endl;
    double TT = 0.0,CW=0.0;
    for(int i = 0; i < plist.size();i++){
        printf("%04d: %4d %4d %4d %4d %1d | %5d %5d %5d %5d\n",
            plist[i]->pid,plist[i]->AT,plist[i]->TC,plist[i]->CB,
            plist[i]->IO,plist[i]->PRIO,plist[i]->FT,plist[i]->TT,
            plist[i]->IT,plist[i]->CW
        );
        CW+=plist[i]->CW;
        TT+=plist[i]->FT-plist[i]->AT;
    }
    // double cpu_util   = 100.0 * (time_cpubusy  / (double) finishtime); 
    // double io_util    = 100.0 * (time_iobusy   / (double) finishtime); 
    // double throughput = 100.0 * (num_processes / (double) finishtime); 

    // printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n",
        
    

    // );





    // }
    

    
    

    return 0;


}