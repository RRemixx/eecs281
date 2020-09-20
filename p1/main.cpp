//Project Identifier: 50EB44D3F029ED934858FFFCEAC3547C68768FC9
#include <vector>
#include <iostream>
#include <algorithm> 
#include <getopt.h>
#include <string>  
#include <cstring>  
#include <sstream>
#include "lettman.h"
using namespace std;


int main(int argc, char** argv){
    letterman t;
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
}
