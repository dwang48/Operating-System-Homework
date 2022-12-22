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

int total_time = 0;
int tot_movement = 0;
int tot_turnaround = 0;
int tot_waittime = 0;
int max_waittime = 0;
int head = 0;
int direction = 1;


struct req{
    int arrival_time;
    int accessed_track;
    int start_time = 0;
    int end_time = 0;
    //initialization
    req(int t, int a){
        arrival_time = t;
        accessed_track = a; 
    }
};

class Scheduler{
    private:
    public:
    virtual void add(req *r) = 0;
    virtual req *poll() = 0;
    virtual bool empty() = 0;


    Scheduler(){};
    ~Scheduler(){};
};

class FIFO: public Scheduler{
    private:
        list<req*> io_list;
    public:
        void add(req *r){
            io_list.push_back(r);
        }
        req* poll(){
            if(io_list.empty()){
                return nullptr;
            }
            else{
                req *temp = io_list.front();
                io_list.pop_front();
                if(temp->accessed_track > head){
                    direction = 1;
                }
                else if(temp->accessed_track < head){
                    direction = -1;
                }
                return temp;
            }
        }
        bool empty(){
            return io_list.size()==0;
        }
};

class SSTF: public Scheduler{
    private:
        list<req*> io_list;
    public:
        void add(req *r){
            io_list.push_back(r);
        }
        req* poll(){
            if(io_list.empty()){
                return nullptr;
            }
            else{
                list<req*>::iterator iter;
                int mi = INT_MAX;
                req *temp;
                list<req*>::iterator erase_iter;
                for(iter = io_list.begin();iter!= io_list.end();iter++){
                    if(abs(head-(*iter)->accessed_track)<mi){
                        mi = abs(head-(*iter)->accessed_track);
                        erase_iter = iter;
                    }
                }
                temp = *erase_iter;
                io_list.erase(erase_iter);
                if(temp->accessed_track > head){
                    direction = 1;
                }
                else if(temp->accessed_track < head){
                    direction = -1;
                }
                return temp;
            }
        }
        bool empty(){
            return io_list.size()==0;
        }
};

class LOOK: public Scheduler{
    private:
        list<req*> io_list;
    public:
        void add(req *r){
            io_list.push_back(r);
        }
        req* poll(){
            if(io_list.empty()){
                return nullptr;
            }
            else{
                list<req*>::iterator iter;
                int mi = INT_MAX;
                req *temp = nullptr;
                bool flag = false;
                int diff;
                list<req*>::iterator erase_iter;
                for(iter = io_list.begin();iter!= io_list.end();iter++){
                    diff = (*iter)->accessed_track-head;
                    if((((diff * direction )>= 0) && abs(diff) < mi)){
                        mi = abs(diff);
                        erase_iter = iter;
                        temp = *erase_iter;
                    }
                }
                if(!temp){
                    direction = -direction;
                    for(iter = io_list.begin();iter!=io_list.end();iter++){
                        diff = (*iter)->accessed_track-head;
                        if((((diff * direction )>= 0) && abs(diff) < mi)){
                            mi = abs(diff);
                            erase_iter = iter;
                        }
                    }
                }
                temp = *erase_iter;
                io_list.erase(erase_iter);
                return temp;
            }
        }
        bool empty(){
            return !io_list.size();
        }
};

class CLOOK:public Scheduler{
    private:
        list<req*> io_list;
    public:
        void add(req *r){
            io_list.push_back(r);
        }
        req* poll(){
            if(io_list.empty()){
                return nullptr;
            }
            else{
                list<req*>::iterator iter;
                int mi = INT_MAX;
                req *temp = nullptr;
                bool flag = false;
                int diff;
                list<req*>::iterator erase_iter;
                for(iter = io_list.begin();iter!= io_list.end();iter++){
                    diff = (*iter)->accessed_track-head;
                    if(((diff >= 0) && diff < mi)){
                        mi = abs(diff);
                        erase_iter = iter;
                        temp = *erase_iter;
                    }
                }
                if(!temp){
                    mi = INT_MAX;
                    for(iter = io_list.begin();iter!=io_list.end();iter++){
                        diff = (*iter)->accessed_track;
                        if(diff < mi){
                            mi = abs(diff);
                            erase_iter = iter;
                        }
                    }
                }
                
                temp = *erase_iter;
                io_list.erase(erase_iter);
                if(temp->accessed_track > head){
                    direction = 1;
                }
                else if(temp->accessed_track < head){
                    direction = -1;
                }
                return temp;
            }
        }
        bool empty(){
            return !io_list.size();
        }
};


class FLOOK: public Scheduler{
    private:
        list<req*> active_list;
        list<req*> io_list;
    public:
        void add(req *r){
            io_list.push_back(r);
        }
        req* poll(){
            req *temp = nullptr;
            list<req*>::iterator erase_iter;
            if(active_list.empty()&&io_list.empty()){
                return nullptr;
            }
            else if(active_list.empty()){
                active_list.swap(io_list);
            }
            list<req*>::iterator iter;
            int mi = INT_MAX;
            
            bool flag = false;
            int diff;
            
            for(iter = active_list.begin();iter!= active_list.end();iter++){
                diff = (*iter)->accessed_track-head;
                if((((diff * direction )>= 0) && abs(diff) < mi)){
                    mi = abs(diff);
                    erase_iter = iter;
                    temp = *erase_iter;
                }
            }
            if(!temp){
                direction = -direction;
                for(iter = active_list.begin();iter!=active_list.end();iter++){
                    diff = (*iter)->accessed_track-head;
                    if((((diff * direction )>= 0) && abs(diff) < mi)){
                        mi = abs(diff);
                        erase_iter = iter;
                    }
                }
            }
            temp = *erase_iter;
            active_list.erase(erase_iter);
            return temp;
        }
        bool empty(){
            return !(io_list.size()&&active_list.size());
        }
};


void simulation(vector<req*> req_vec, Scheduler* s){
    
    int idx = 0;
    int current_time = 0;
    req *r = nullptr;
    int remain = req_vec.size();
    while(true){
        // if a new I/O arrived at the system at this current time
        // → add request to IO-queue
        if(idx < req_vec.size() && req_vec[idx]->arrival_time==current_time){
            s->add(req_vec[idx++]);
            remain--;
        }
        // if an IO is active and completed at this time
        if(r&&r->accessed_track == head){
            //cout<<"active"<<endl;
            // → Compute relevant info and store in IO request for final summary
            r->end_time = current_time;
            tot_turnaround += r->end_time - r->arrival_time;
            tot_waittime += r->start_time - r->arrival_time;
            max_waittime = max(max_waittime, r->start_time-r->arrival_time);
            r = nullptr;
        }
        // if an IO is active
        // → Move the head by one unit in the direction its going (to simulate seek)
        else if(r){
            head += direction;
        }
        //if no IO is active
        if(!r){
        // if requests are pending
        // → Fetch the next request from IO-queue and start the new IO.
            //cout<<"run"<<endl;
            r = s->poll();
            if(r){
                r->start_time = current_time;
                tot_movement += abs(head - r->accessed_track);
                continue;
            }
            // else if all IO from input file processed
            // → exit simulation
            if(s->empty()&&remain==0){
                break;
            }
        }
        current_time++;
        total_time = current_time;
    }
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

    vector<req*> req_vec;
    
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
        req *r = new req(res[0],res[1]);
        req_vec.push_back(r);
    }
    simulation(req_vec,s);

    double avg_turnaround = (double)tot_turnaround/req_vec.size();
    double avg_waittime = (double)tot_waittime/req_vec.size();
    
    for(int i = 0; i < req_vec.size(); i++){
        req* r = req_vec[i];
        printf("%5d: %5d %5d %5d\n",i, r->arrival_time, r->start_time, r->end_time);
    }

    printf("SUM: %d %d %.2lf %.2lf %d\n",
    total_time, tot_movement, avg_turnaround, avg_waittime, max_waittime);


    return 0;



}