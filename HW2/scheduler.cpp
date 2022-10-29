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
RUNNING,
BLOCKED,
READY,
FINISHED
} process_state_t;

typedef enum{
    TRANS_TO_READY,
    TRANS_TO_PREEMPT,
    TRANS_TO_RUN,
    TRANS_TO_BLOCK
} trans_state_t;






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
            return nullptr; 
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
            list<event*>::iterator<event*> it;
            for(it = q.begin();it!=q.end();++it){
                if(it->t>e->t){
                    q.insert(it,e);
                    break;
                }
            }
            if(it==q.end()){
                q.push_back(e);
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
    void add_process(process *p);
    process* get_next_process();
    bool does_preempt();

private:

};


//FCFS,  LCFS,  SRTF,  RR  (RoundRobin),  PRIO  (PriorityScheduler)  and  PREemptive  PRIO  (PREPRIO)
enum{
    FCFS,LCFS,SRTF,RR,PRIO,PREPRIO
};

class FCFS:public scheduler{
    private:
        list<process*> q;
    public:
        void add_process(process* p){
            p->process_state = READY;

        }
        process* get_next_process(){

        }
};
class LCFS:public scheduler{
    private:
        list<process*> q;
};
class SRTF:public scheduler{
    private:
        list<process*> q;
};
class RR:public scheduler{
    private:
        list<process*> q;
};
class PRIO:public scheduler{
private:
    list<process *> active;
	list<process *> expired;
public:

};
class PREPRIO:public scheduler{
private:
    list<process *> active;
	list<process *> expired;

public:



};

void print_event(){

}

void simulation(scheduler * sched, event_list * el, int quantum,){
    event* evt;
    int current_time;
    bool call_scheduler = false;
    process * current_running_process = nullptr;
    while((evt = el->get_event())){
        process *p = evt->p;
        current_time = evt->t;
        int transition = evt->tran;
        int timeInPrevState = current_time - proc->state_ts;

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

    for(;;){
        switch(getopt(argc,argv,"vteps:")){
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
                cout<<mode<<endl;
                break;
        }
    }
   
   

   switch(mode[0]){
    case 'F':
        break;
    case 'L':
        break;
    case 'S':
        break;
    case 'R':
        break;
    case 'P':
        string:size_type delim = mode.find(':');


        break;
    case 'E':
        break;
    default:
        break;
   
   }
   
   
    // printf("%04d: %4d %4d %4d %4d %1d | %5d %5d %5d %5d\n",
    // printf(“SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n",       

    simulation();
    
    
    return 0;


}