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

using namespace std;

struct Event{

}

struct Process{
    int AT; // arrival time
    int TC; // total cpu time
    int CB; // CPU Burst
    int IO; // IO Burst
    init(){

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
    void add_process(Process *p);
    Process* get_next_process();
    bool does_preempt();

private:

};


//FCFS,  LCFS,  SRTF,  RR  (RoundRobin),  PRIO  (PriorityScheduler)  and  PREemptive  PRIO  (PREPRIO)
class FCFS::scheduler{

}
class LCFS::scheduler{

}
class SRTF::scheduler{

}
class RR::scheduler{

}
class PRIO::scheduler{

}
class PREPRIO::scheduler{

}

void print_event(){

}

void simulation(){

}








int main(int argc, char* argv[]){


}