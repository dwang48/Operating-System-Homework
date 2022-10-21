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

struct event{
    int t; //time
    process *p; //process
    process_state_t prev; // prev state
    process_state_t next; // next state
    trans_state_t tran; // transition
};

struct event_list{
    list<event *> l;
    event *front(){
        if(!l.empty()){
            return nullptr; 
        }
        else{
            return l.front();
        }
    }
    event *poll(){
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

        }
    }
};


struct process{
    int AT; // arrival time
    int TC; // total cpu time
    int CB; // CPU Burst
    int IO; // IO Burst
    void init(int arrival_time, int total_cpu_time, int CPU_burst, int IO_burst){
        AT = arrival_time;
        TC = total_cpu_time;
        CB = CPU_burst;
        IO = IO_burst;
    };
};

typedef enum{
RUNNING,
BLOCKED
} process_state_t;

typedef enum{
    TRANS_TO_READY,
    TRANS_TO_PREEMPT,
    TRANS_TO_RUN,
    TRANS_TO_BLOCK
} trans_state_t;



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
    public:
    private:
        queue<int> q;
};
class LCFS:public scheduler{

};
class SRTF:public scheduler{

};
class RR:public scheduler{

};
class PRIO:public scheduler{

};
class PREPRIO:public scheduler{

};

void print_event(){

}

void simulation(){

}








int main(int argc, char* argv[]){
   
   
   
   
   
    // printf("%04d: %4d %4d %4d %4d %1d | %5d %5d %5d %5d\n",
    // printf(“SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n",       



}