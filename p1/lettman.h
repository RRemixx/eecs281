//Project Identifier: 50EB44D3F029ED934858FFFCEAC3547C68768FC9

#include <vector>
#include <iostream>
#include <algorithm> 
#include <getopt.h>
#include <string>  
#include <cstring>  
#include <sstream>
#include <deque>

using namespace std;

struct word{
    string diff;
    string name;
    int prev=-1;
    bool discovered=false;
};


class letterman{
    private:
        vector<word> dic;
        deque<string> result;
        deque<int> searcher;
        string beginword;
        string endword;
        int num_arg=0;   //number of -s and -q and begin word and end word
        int cur_index;
        int num_lines;
        int num_words;
        bool is_change=false;  //letterman can change or not
        bool is_swap=false;    //letterman can swap or not
        bool is_length=false;  //letterman can insert and delete or not
        bool is_stack=false;   //use a stack otherwise use a queue
        bool is_wordformat=true; //output format by default word format

        //convert string to int
        int str2int(string &input){
            int res_num=0;
            istringstream iss(input);
            iss>>res_num;
            return res_num;
        } 

        bool get_simple_line(string line){
            //if not comment
            if((line.length()>1&&(line[0]=='/'&&line[1]=='/'))){
                return false;
            }
            else{
                word newword;
                newword.discovered=false;
                newword.name=line;
                dic.push_back(newword);
            }
            return true;
        }

        bool get_complex_line(string line){
            //if not comment
            if((line.length()>1&&(line[0]=='/'&&line[1]=='/'))){
                return false;
            }
            else{
                //&
                if(line[line.length()-1]=='&'){
                    line.erase(line.length()-1,1);  //delete &
                    word newword;
                    newword.name=line;
                    dic.push_back(newword);
                    reverse(line.begin(),line.end()); //reverse word
                    newword.name=line;
                    dic.push_back(newword);
                    num_words+=2;
                    return true;
                }
                //!
                int pos=(int)line.find("!");
                if(pos!=(int)line.npos){
                    line.erase(pos,1);  //erase !
                    word newword;
                    newword.name=line;
                    dic.push_back(newword);
                    char swaper=line[pos-1];  //swap
                    line[pos-1]=line[pos-2];
                    line[pos-2]=swaper;
                    newword.name=line;
                    dic.push_back(newword);
                    num_words+=2;
                    return true;
                }
                //?
                pos=(int)line.find("?");
                if(pos!=(int)line.npos){
                    word word_one,word_two;
                    line[pos]=line[pos-1];
                    word_two.name=line;
                    word_one.name=line.erase(pos,1);
                    dic.push_back(word_one);
                    dic.push_back(word_two);
                    num_words+=2;
                    return true;
                }
                //[]
                pos=(int)line.find("[");
                int pos2=(int)line.find("]");
                if(pos!=(int)line.npos&&pos2!=(int)line.npos){
                    word newword;
                    string initial_line=line;
                    int num_words_thisline=pos2-pos-1;
                    line.erase(pos,num_words_thisline+1);
                    string word_name=line;
                    for(int i=0;i<num_words_thisline;i++){
                        word_name[pos]=initial_line[pos+i+1];
                        newword.name=word_name;
                        dic.push_back(newword);
                        num_words++;
                    }
                }
            }
            return true;
        }

        bool change_word(word &checking_word,int index,int length){
            string diff="";
            string dicword=checking_word.name;
            string cur_word=dic[cur_index].name;
            bool changed=false;
            for(int i=0;i<(int)length;i++){
                if(changed&&(dicword[i]!=cur_word[i])){
                    changed=false;
                    break;
                }
                if((!changed)&&(dicword[i]!=cur_word[i])){
                    changed=true;
                    if(!is_wordformat){
                        ostringstream diff_oss;
                        diff_oss<<"c,"<<i<<","<<dicword[i];
                        diff=diff_oss.str();
                    } 
                }
            }
            if(changed){
                checking_word.discovered=true;
                checking_word.diff=diff;
                checking_word.prev=cur_index;
                searcher.push_back(index);
            }
            return changed;
        }

        bool swap_word(word &checking_word,int index,int length){
            string diff="";
            string dicword=checking_word.name;
            string cur_word=dic[cur_index].name;
            bool changed=false;
            for(int i=0;i<(int)length;i++){
                if(dicword[i]!=cur_word[i]){
                    if(changed){ //after the first difference, no difference allowed
                        changed=false;
                        break;
                    }
                    if(dicword[i+1]==cur_word[i]&&dicword[i]==cur_word[i+1]){
                        changed=true;
                        if(is_wordformat){
                            ostringstream diff_oss;
                            diff_oss<<"s,"<<i;
                            diff=diff_oss.str();
                        }
                        i++;
                    }
                }
            }
            if(changed){
                checking_word.discovered=true;
                checking_word.diff=diff;
                checking_word.prev=cur_index;
                searcher.push_back(index);
            }
            return changed;
        }


    bool delete_word(word &checking_word,int index,int length){
        string diff="";
        string dicword=checking_word.name;
        string cur_word=dic[cur_index].name;
        bool changed=false;
        //delete from cur_word
        for(int i=0;i<=length+1;i++){
            //last char is deleted
            if(!changed){
                if(i==length+1){
                    if(is_wordformat){
                        ostringstream diff_oss;
                        diff_oss<<"d,"<<i;
                        diff=diff_oss.str();
                    }
                    break;
                }
                if(dicword[i]!=cur_word[i]){
                    changed=true;
                    if(is_wordformat){
                        ostringstream diff_oss;
                        diff_oss<<"d,"<<i;
                        diff=diff_oss.str();
                    }
                }
            }
            else{
                if(dicword[i-1]!=cur_word[i]){
                    return false;
                }
            }
        }
        checking_word.discovered=true;
        checking_word.diff=diff;
        checking_word.prev=cur_index;
        searcher.push_back(index);
        return true;
    }

    bool insert_word(word &checking_word,int index,int length){
        string diff="";
        string dicword=checking_word.name;
        string cur_word=dic[cur_index].name;
        bool changed=false;
        for(int i=0;i<=length;i++){
            if(!changed){
                if(i==length){
                    if(!is_wordformat){
                        ostringstream diff_oss;
                        diff_oss<<"i,"<<i<<","<<dicword[i];
                        diff=diff_oss.str();
                    }  
                    break;
                }
                if(dicword[i]!=cur_word[i]){
                    changed=true;
                    if(!is_wordformat){
                        ostringstream diff_oss;
                        diff_oss<<"i,"<<i<<","<<dicword[i];
                        diff=diff_oss.str();
                    }  
                }
            }
            else{
                if(dicword[i]!=cur_word[i-1]){
                    return false;
                }
            }
        }
        checking_word.discovered=true;
        checking_word.diff=diff;
        checking_word.prev=cur_index;
        searcher.push_back(index);
        return true;
    }

    public:

        void printDic(){
            for(int i=0;i<num_words;i++){
                cout<<dic[i].name<<endl;
            }
        }
        
        void getArg(int argc,char* argv[]); 

        void getDic();

        bool check_word(int index);   //if endword is added, returns true.
        bool check_words(int index);   //new check_word

        int update_searcher();    //if endword is added, returns 1. if searcher is empty, returns 0. otherwise returns 2.

        void getRes();    //if endword is found, generate the deque result

        int get_num_discovered();    //return the number of discovered words in dictionary

        //test functions
        void printStack(){
            string lll;
            deque<int> newsea(searcher);
            cout<<"Now searcher is: ";
            while(!newsea.empty()){
                lll=dic[newsea.back()].name;
                cout<<lll<<" ";
                newsea.pop_back();
            }
            cout<<endl;
        }
};


void letterman::getArg(int argc,char* argv[]){
    int option_index = 0, option = 0;
    opterr = false;
    struct option longOpts[] = {
        { "stack", no_argument, nullptr, 's' },
        {"queue",no_argument,nullptr,'q'},
        {"change",no_argument,nullptr,'c'},
        {"swap",no_argument,nullptr,'s'},
        {"length",no_argument,nullptr,'l'},
        {"output",required_argument,nullptr,'o'},
        {"begin",required_argument,nullptr,'b'},
        {"end",required_argument,nullptr,'e'},
        { "help", no_argument, nullptr, 'h' },
        { nullptr, 0, nullptr, '\0' }
    };
    while ((option = getopt_long(argc, argv, "sqcplo:b:e:h", longOpts, &option_index)) != -1) {
        switch (option) {
            case 's': 
                is_stack=true;
                num_arg++;
                break;
            case 'q':
                is_stack=false;
                num_arg++;
                break;
            case 'c':
                is_change=true;
                break;
            case 'p':
                is_swap=true;
                break;
            case 'l':
                is_length=true;
                break;
            case 'o':
                if(!strcmp("M",optarg)){
                    is_wordformat=false;
                }
                else{
                    if(!strcmp("W",optarg)){
                        is_wordformat=true;
                    }else{
                        cerr<<"output format invalid.\n";
                        exit(1);
                    }
                }
                break;
            case 'b':
                beginword=optarg;
                num_arg++;
                break;
            case 'e':
                endword=optarg;
                num_arg++;
                break;
            case 'h':
                cout<<"This program read from a input file to get the dictionary, then use a queue-based or stack-based routing scheme to find a path from a begin word to a end word.\n"
                <<"The user can specify the routing scheme, how word can be changed the output format, the begin word and the end word in the command line.\n"
                <<"--stack, -s​: If this switch is set, use the stack-based routing scheme\n"
                <<"--queue, -q:​ If this switch is set, use the queue-based routing scheme.\n"
                <<"--change, -c:​ If this switch is set, Letterman is allowed to change one letter into another.\n"  
                <<"--swap, -p:​ If this switch is set, Letterman is allowed to swap any two adjacent characters.\n"
                <<"--length, -l:​ If this switch is set, Letterman is allowed to modify the length of a word, by inserting or deleting a single letter.\n"
                <<"--output (W|M),  -o (W|M):​ Indicates the output file format by following the flag with a ​W (word format) or ​M​ (modification format).  If the --output option is not specified, default to word output format (W).  If --output is specified on the command line, the argument (either ​W​ or ​M​) to it is required.\n"
                <<"--begin <word>, -b <word>:​ This specifies the word that Letterman starts with.  This flag must be specified on the command line, and when it is specified a word must follow it. \n"
                <<"--end <word>, -e <word>:​ This specifies the word that Letterman must reach.  This flag must be specified on the command line, and when it is specified a word must follow it.\n"
                <<"--help, -h:​ If this switch is set, the program should print a brief help message which describes what the program does and what each of the flags are.\n";
                exit(0);
        }
    }

    if(num_arg!=3){
        cerr<<"Wrong num of -s-q or begin and end word.\n";
        exit(1);
    }
    if((!is_change)&&(!is_swap)&&(!is_length)){
        cerr<<"letterman can do nothing!\n";
        exit(1);
    }
    if(!(is_length)&&beginword.length()!=endword.length()){
        cerr<<"letterman cannot help.\n";
        exit(1);
    }
}


void letterman::getDic(){
    int dic_size=0; //record current dictionary size
    string type;  //simple or complex
    string dic_line; //read from input
    //get type
    getline(cin,type);
    //get num of lines
    getline(cin,dic_line);
    num_lines=str2int(dic_line);
    dic.reserve((size_t)num_lines);
    //simple dictionary
    if("S"==type){
        num_words=num_lines;
        while(dic_size<num_lines){
            getline(cin,dic_line);
            if(get_simple_line(dic_line)){
                dic_size++;
            }
        }
    }
    else{ //complex dictionary
        num_words=0;
        while(dic_size<num_lines){
            getline(cin,dic_line);
            if(get_complex_line(dic_line)){
                dic_size++;
            }
        }
    }
    for(int i=0;i<num_words;i++){
        if(dic_size==-2){
            break;
        }
        if(dic[i].name==beginword){
            dic[i].discovered=true;
            dic[i].diff=beginword;
            cur_index=i;
            dic_size=-1;
        }
        if(dic[i].name==endword){
            dic_size--;
        }
    }
    if(dic_size!=-2){
        cerr<<"begin word or end word missing\n";
        exit(1);
    }
}


bool letterman::check_words(int index){
    if(dic[index].discovered){return false;}
    else{
        word checking_word=dic[index];
        int length=(int)checking_word.name.length();
        int cur_length=(int)dic[cur_index].name.length();
        bool is_end=(endword==checking_word.name);
        if(length==cur_length){
            if(is_change&&change_word(checking_word,index,length)){
                return is_end;
            }
            if(is_swap&&swap_word(checking_word,index,length)){
                return is_end;
            }
        }
        else{
            if(is_length){
                if(length==cur_length+1&&insert_word(checking_word,index,length)){
                    return is_end;
                }
                if(length==cur_length-1&&insert_word(checking_word,index,length)){
                    return is_end;
                }
            }
            
        }
    }
    return false;
}


bool letterman::check_word(int index){
    if(dic[index].discovered){return false;}
    else{
        ostringstream diff_oss;
        bool changed=false;  //whether changed once already
        string dicword=dic[index].name;
        string cur_word=dic[cur_index].name;
        string diff_str;     //difference with current word
        size_t length=dicword.length();
        //mode change and swap
        if(length==cur_word.length()){
            if(is_change){
                for(int i=0;i<(int)length;i++){
                    if(changed&&(dicword[i]!=cur_word[i])){
                        changed=false;
                        break;
                    }
                    if((!changed)&&(dicword[i]!=cur_word[i])){
                        changed=true;
                        diff_oss<<"c,"<<i<<","<<dicword[i];
                    }
                }
                if(changed){
                    dic[index].discovered=true;
                    dic[index].diff=diff_oss.str();
                    dic[index].prev=cur_index;
                    searcher.push_back(index);
                    if(endword==dicword){
                        return true;
                    }
                    return false;
                }
            }
            if(is_swap){
                for(int i=0;i<(int)length;i++){
                    if(dicword[i]!=cur_word[i]){
                        if(changed){ //after the first difference, no difference allowed
                            changed=false;
                            break;
                        }
                        if(dicword[i+1]==cur_word[i]&&dicword[i]==cur_word[i+1]){
                            changed=true;
                            diff_oss<<"s,"<<i;
                            i++;
                        }
                    }
                }
                if(changed){
                    dic[index].discovered=true;
                    dic[index].diff=diff_oss.str();
                    dic[index].prev=cur_index;
                    searcher.push_back(index);
                    if(endword==dicword){
                        return true;
                    }
                    return false;
                }
            }
        }
        else{ //delete and insert 
            if(is_length){
                //delete from cur_word
                if(length==cur_word.length()-1){
                    changed=false;
                    for(int i=0;i<=(int)length+1;i++){
                        //last char is deleted
                        if(!changed){
                            if(i==(int)length+1){
                                diff_oss<<"d,"<<i;
                                break;
                            }
                            if(dicword[i]!=cur_word[i]){
                                changed=true;
                                diff_oss<<"d,"<<i;
                            }
                        }
                        else{
                            if(dicword[i-1]!=cur_word[i]){
                                return false;
                            }
                        }
                    }
                    dic[index].discovered=true;
                    dic[index].diff=diff_oss.str();
                    dic[index].prev=cur_index;
                    searcher.push_back(index);
                    if(endword==dicword){
                        return true;
                    }
                    return false;
                }
                //insert from cur_word
                if(length==cur_word.length()+1){
                    changed=false;
                    for(int i=0;i<=(int)length;i++){
                        if(!changed){
                            if(i==(int)length){
                                diff_oss<<"i,"<<i<<","<<dicword[i];
                                break;
                            }
                            if(dicword[i]!=cur_word[i]){
                                changed=true;
                                diff_oss<<"i,"<<i<<","<<dicword[i];
                            }
                        }
                        else{
                            if(dicword[i]!=cur_word[i-1]){
                                return false;
                            }
                        }
                    }
                    dic[index].discovered=true;
                    dic[index].diff=diff_oss.str();
                    dic[index].prev=cur_index;
                    searcher.push_back(index);
                    if(endword==dicword){
                        return true;
                    }
                    return false;
                }
            }
            
        }
    }
    return false;
}


int letterman::update_searcher(){
    for(int i=0;i<num_words;i++){
        if(check_word(i)){
            return 1;
        }
    }
    if(searcher.empty()){
        return 0;
    }
    if(is_stack){
        cur_index=searcher.back();
        searcher.pop_back();
    }
    else{
        cur_index=searcher.front();
        searcher.pop_front();
    }
    return 2;
}


void letterman::getRes(){
    for(int i=0;i<num_words;i++){
        if(endword==dic[i].name){
            if(is_wordformat){
                word prev_word=dic[i];
                result.push_back(prev_word.name);
                while(prev_word.name!=beginword){
                    prev_word=dic[prev_word.prev];
                    result.push_back(prev_word.name);
                }
            }
            else{
                word prev_word=dic[i];
                result.push_back(prev_word.diff);
                while(prev_word.name!=beginword){
                    prev_word=dic[prev_word.prev];
                    result.push_back(prev_word.diff);
                }
            }
            break;
        }
    }
    //output
    cout<<"Words in morph: "<<result.size()<<'\n';
    while(!result.empty()){
        cout<<result.back()<<'\n';
        result.pop_back();
    }
}


int letterman::get_num_discovered(){
    int res_num=0;
    for(int i=0;i<num_words;i++){
        if(dic[i].discovered){
            res_num++;
        }
    }
    return res_num;
}