// Project Identifier: 50EB44D3F029ED934858FFFCEAC3547C68768FC9
#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <deque>
using namespace std;

struct Word {
    string str;
    int prev_index = -1;
    bool is_discovered = false;
};

class Letterman {
    public:
        void get_options(int argc, char** argv);
        void read_dictionary();
        void read_dictionary_helper(string const & word);
        int find_index(string const & str);
        void check_existence();
        static int check_change(string const & word1, string const & word2);
        static int check_swap(string const & word1, string const & word2);
        static int check_length(string const & word1, string const & word2);
        bool add_similar_words(string const & current_word);
        void add_similar_words_helper(int const & index, string const & str);
        void run();
        void display_output();

    private:
        vector<Word> dictionary;
        deque <string> word_deque;
        stack <string> solution;
        char routing_scheme = '\0';
        char output_format = 'W';
        bool no_solution = false;
        bool change_enabled = false, swap_enabled = false, length_enabled = false;
        string begin_word, end_word;
        int begin_word_index, end_word_index;
};

int main(int argc, char** argv){
    ios_base::sync_with_stdio(false);
    Letterman letterman;
    letterman.get_options(argc, argv);
    letterman.read_dictionary();
    letterman.check_existence();
    letterman.run();
    letterman.display_output();
}

void Letterman::get_options(int argc, char** argv){
    int num_scheme = 0;
    bool begin_word_specified = false, end_word_specified = false;

    int option_index = 0, option = 0;
    opterr = false;
    struct option longOpts[] = {{ "stack", no_argument, nullptr, 's' },
                                { "queue", no_argument, nullptr, 'q' },
                                { "change", no_argument, nullptr, 'c' },
                                { "swap", no_argument, nullptr, 'p' },
                                { "length", no_argument, nullptr, 'l' },
                                { "output", required_argument, nullptr, 'o' },
                                { "begin", required_argument, nullptr, 'b' },
                                { "end", required_argument, nullptr, 'e' },
                                { "help", no_argument, nullptr, 'h' },
                                { nullptr, 0, nullptr, '\0' }};

    while ((option = getopt_long(argc, argv, "sqcplo:b:e:h", longOpts, &option_index)) != -1) {
        switch (option) {
            case 's':
                routing_scheme = 's';
                num_scheme++;
                break;
            case 'q':
                routing_scheme = 'q';
                num_scheme++;
                break;
            case 'c':
                change_enabled = true;
                break;
            case 'p':
                swap_enabled = true;
                break;
            case 'l':
                length_enabled = true;
                break;
            case 'o':
                output_format = *optarg;
                if(output_format != 'W' && output_format != 'M'){
                    cerr << "Error: The --output/-o flag is followed by an invalid character " << output_format << '\n';
                    exit(1);
                }
                break;
            case 'b':
                begin_word = optarg;
                begin_word_specified = true;
                break;
            case 'e':
                end_word = optarg;
                end_word_specified = true;
                break;
            case 'h':
                cout << "The help function is called.\n";
                exit(0);
        }
    }
    if(num_scheme != 1){
        cerr << "Error: More or less than one --stack/-s or --queue/-q on the command line.\n";
        exit(1);
    }
    if(!change_enabled && !swap_enabled && !length_enabled){
        cerr << "Error: No --change/-c, --length/-l, or --swap/-p on the command line.\n";
        exit(1);
    }
    if(!begin_word_specified){
        cerr << "Error: The start word is not specified.\n";
        exit(1);
    }
    if(!end_word_specified){
        cerr << "Error: The end word is not specified.\n";
        exit(1);
    }
    if(!length_enabled && begin_word.length() != end_word.length()){
        cerr << "Error: --length/-l is not specified, and the start/end words do not match in length.\n";
        exit(1);
    }
}

void Letterman::read_dictionary(){
    char type;
    int num_word;
    string line;
    getline(cin, line);
    type = line[0];
    getline(cin, line);
    num_word = stoi(line);

    if(type == 'S'){
        while(num_word > 0){
            getline(cin, line);
            if(line[0] != '/' || line[1] != '/'){
                if(length_enabled || line.length() == begin_word.length()){
                    Word new_word;
                    new_word.str = line;
                    dictionary.push_back(new_word);
                }
                num_word--;
            }
        }
    }
    else if(type == 'C'){
        while(num_word > 0){
            getline(cin, line);
            if(line[0] != '/' || line[1] != '/'){
                read_dictionary_helper(line);
                num_word--;
            }
        }
    }
}

void Letterman::read_dictionary_helper(string const & word){
    int num_word = 1;
    Word new_word1, new_word2;
    string word1_str, word2_str;
    bool has_brackets = false;

    for(unsigned int i = 0; i < word.length(); i++){
        if(word[i] == '!'){
            if(!length_enabled && word.length() - begin_word.length() != 1)
                return;
            word1_str = word.substr(0, i) + word.substr(i + 1, word.length() - i - 1);
            word2_str = word.substr(0, i - 2) + word[i - 1] + word[i - 2] + word.substr(i + 1, word.length() - i - 1);
            num_word++;
            break;
        }
        else if(word[i] == '?'){
            word1_str = word.substr(0, i) + word.substr(i + 1, word.length() - i - 1);
            word2_str = word.substr(0, i) + word[i - 1] + word.substr(i + 1, word.length() - i - 1);
            num_word++;
            break;
        }
        else if(word[i] == '&' && i == word.length() - 1){
            if(!length_enabled && word.length() - begin_word.length() != 1)
                return;
            word1_str = word.substr(0, word.length() - 1);
            for(int j = (int) word.length() - 2; j >= 0; j--){
                word2_str += word[j];
            }
            num_word++;
            break;
        }
        else if(word[i] == '['){
            unsigned int j = i + 1;
            while(word[j] != ']'){
                j++;
            }
            string inside_bracket = word.substr(i + 1, j - i - 1);
            int word_length = i - j + (int)word.length();
            if(!length_enabled && word_length != (int)begin_word.length())
                return;
            for(unsigned int k = 0; k < inside_bracket.length(); k++){
                word1_str = word.substr(0, i) + inside_bracket[k] + word.substr(j + 1, word.length() - j - 1);
                new_word1.str = word1_str;
                dictionary.push_back(new_word1);
            }
            has_brackets = true;
            break;
        }
    }
    if(num_word == 1 && !has_brackets){
        new_word1.str = word;
        dictionary.push_back(new_word1);
    }
    else if(num_word == 2){
        new_word1.str = word1_str;
        new_word2.str = word2_str;
        dictionary.push_back(new_word1);
        dictionary.push_back(new_word2);
    }
}

int Letterman::find_index(string const & str){
    for(unsigned int i = 0; i < dictionary.size(); i++){
        if(dictionary[i].str == str)
            return (int)i;
    }
    return -1;
}

void Letterman::check_existence(){
    begin_word_index = find_index(begin_word);
    end_word_index = find_index(end_word);
    if(begin_word_index == -1){
        cerr << "Error: The start word does not exist in the dictionary.\n";
        exit(1);
    }
    if(end_word_index == -1){
        cerr << "Error: The end word does not exist in the dictionary.\n";
        exit(1);
    }
}

int Letterman::check_change(string const & word1, string const & word2){
    if(word1.length() == word2.length()){
        unsigned int index = 0;
        int num_dif = 0;
        for(unsigned int i = 0; i < word1.length(); i++){
            if(word1[i] != word2[i]){
                index = i;
                num_dif++;
            }
            if(num_dif > 1)
                return -1;
        }
        return (int)index;
    }
    return -1;
}

int Letterman::check_swap(string const & word1, string const & word2){
    if(word1.length() == word2.length()){
        for(unsigned int i = 0; i < word1.length(); i++){
            if(word1[i] != word2[i]){
                if(word1[i] == word2[i + 1] && word1[i + 1] == word2[i]){
                    for(unsigned int j = i + 2; j < word1.length(); j++){
                        if(word1[j] != word2[j])
                            return -1;
                    }
                    return (int)i;
                }
                return -1;
            }
        }
    }
    return -1;
}

int Letterman::check_length(string const & word1, string const & word2){
    if(word1.length() - word2.length() == 1){
        for(unsigned int i = 0; i < word2.length(); i++){
            if(word1[i] != word2[i]){
                for(unsigned int j = i; j < word2.length(); j++){
                    if(word1[j + 1] != word2[j])
                        return -1;
                }
                return (int)i;
            }
        }
        return (int)word2.length();
    }
    else if(word2.length() - word1.length() == 1){
        return check_length(word2, word1);
    }
    return -1;
}

bool Letterman::add_similar_words(string const & current_word){
    bool isAdd = false;
    for(unsigned int i = 0; i < dictionary.size(); i++){
        if(!dictionary[i].is_discovered){
            if(change_enabled && check_change(dictionary[i].str, current_word) >= 0){
                add_similar_words_helper(i, current_word);
                isAdd = true;
            }
            else if(swap_enabled && check_swap(dictionary[i].str, current_word) >= 0){
                add_similar_words_helper(i, current_word);
                isAdd = true;
            }
            else if(length_enabled && check_length(dictionary[i].str, current_word) >= 0){
                add_similar_words_helper(i, current_word);
                isAdd = true;
            }
            if(isAdd && dictionary[i].str == end_word)
                return true;
        }
        isAdd = false;
    }
    return false;
}

void Letterman::add_similar_words_helper(int const & index, string const & str){
    dictionary[index].prev_index = find_index(str);
    dictionary[index].is_discovered = true;
    if(routing_scheme == 'q'){
        word_deque.push_back(dictionary[index].str);
    }
    else if(routing_scheme == 's'){
        word_deque.push_front(dictionary[index].str);
    }
}

void Letterman::run(){
    string current_word;
    dictionary[begin_word_index].is_discovered = true;
    if(routing_scheme == 'q'){
        word_deque.push_back(begin_word);
    }
    else if(routing_scheme == 's'){
        word_deque.push_front(begin_word);
    }

    while(1){
        if(word_deque.empty()){
            no_solution = true;
            break;
        }
        current_word = word_deque.front();
        word_deque.pop_front();
        bool loop_end = add_similar_words(current_word);
        if(loop_end) break;
    }
}

void Letterman::display_output(){
    if(no_solution){
        int num_discovered = 0;
        for(unsigned int i = 0; i < dictionary.size(); i++){
            if(dictionary[i].is_discovered)
                num_discovered++;
        }
        cout << "No solution, " << num_discovered << " words discovered.\n";
        return;
    }

    int index = end_word_index;
    while(index != -1){
        solution.push(dictionary[index].str);
        index = dictionary[index].prev_index;
    }
    cout << "Words in morph: " << solution.size() << '\n';

    cout << begin_word << '\n';
    solution.pop();

    if(output_format == 'W'){
        while(!solution.empty()){
            cout << solution.top() << '\n';
            solution.pop();
        }
    }
    else if(output_format == 'M'){
        int position;
        string previous = begin_word;
        while(!solution.empty()){
            string current = solution.top();
            solution.pop();
            if(previous.length() == current.length()){
                position = check_change(previous, current);
                if(position >= 0){
                    cout << "c," << position << ',' << current[position] << '\n';
                }
                else{
                    position = check_swap(previous, current);
                    cout << "s," << position << '\n';
                }
            }
            else if(previous.length() - current.length() == 1){
                position = check_length(previous, current);
                cout << "d," << position << '\n';
            }
            else if(current.length() - previous.length()  == 1){
                position = check_length(previous, current);
                cout << "i," << position << ',' << current[position] << '\n';
            }
            previous = current;
        }
    }
}