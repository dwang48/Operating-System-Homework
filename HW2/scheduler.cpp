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

typedef enum{
RUNNING,
BLOCKED
} process_state_t;

enum{

}

class scheduler{
public:
    void add_process(Process *p);
    Process* get_next_process();
    bool does_preempt();

private:


};



int main(int argc, char* argv[]){


}