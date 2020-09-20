//Project Identifier: 50EB44D3F029ED934858FFFCEAC3547C68768FC9
#include <vector>
#include <iostream>
#include <algorithm> 
#include <getopt.h>
#include <string>  
#include <cstring>  
#include <sstream>
#include <time.h>
#include "lettman.h"
using namespace std;


int main(int argc, char** argv){
    time_t t1,t2;
    letterman t;
    t1=time(NULL);
    t.getArg(argc,argv);
    t.getDic();
    int k=2;
    while(k==2){
        k=t.update_searcher();
    }
    if(k==1){
        t.getRes();
    }
    else{
        cout<<"No solution, "<<t.get_num_discovered()<<" words discovered\n";
    }
    t2=time(NULL);
    cout<<t2-t1<<endl;
}
