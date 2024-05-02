#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <thread>
#include <chrono>
#include <ctime>
using namespace std;
int num = 0;
bool active = true;
string sword = "cyw43.h";
string path = "/home/david/";
float timeout = 666.0;
vector<string> exceptions = {"sys", ".speech-dispatcher", ".wine", "llvm-14", "proc", "X11"};
vector<string> results;
void checker(){
    float timet = 0;
    while(active){
        this_thread::sleep_for(0.1s);
        timet += 0.1;
        if(timet >= timeout){
            active = false;
            break;
        }
    }
}
string mlower(string str){
    for(char & stabe : str){
        stabe = tolower(stabe);
    }
    return str;
}
bool check_it(string inp, vector<string> inplist){
    bool h = true;
    for(string element : inplist){
        if(mlower(element) == mlower(inp)){
            return false;
            h = false;
            break;
        }
    }
    if(h){return true;}
}
vector<string> split_str(string tos, char del){
    vector<string> re;
    string h;
    for(auto& stabe : tos){
        if(stabe != del){
        h = h + stabe;}
        else if(stabe == del){
            re.push_back(h);
            h = "";
        }
    }
    if(tos[tos.size()] != del){
        re.push_back(h);
    }
    return re;
}
string wend(string str){
    vector<string> sstr = split_str(str, '.');
    return sstr[0];
}
void search_folder(string folder){
    if(active){
        try{
    vector<string> files;
    vector<string> folders;
    for (const auto & entry : filesystem::directory_iterator(folder))
        if(filesystem::is_regular_file(entry)){
             files.push_back(entry.path().c_str());
        }
        else if(filesystem::is_directory(entry)){
             folders.push_back(entry.path().c_str());
        }     
    for(string fpath : files){
        vector<string> filename  = split_str(fpath, '/');
        if(mlower(wend(filename[filename.size()-1])) == mlower(wend(sword))){
            results.push_back(fpath);
        }
        num++;
    }
    for(string folder : folders){
        vector<string> foldername = split_str(folder, '/');
        if(mlower(foldername[foldername.size()-1]) == mlower(wend(sword))){
            results.push_back(folder);
            cout<<folder<<endl;}   
        if(check_it(foldername[foldername.size()-1], exceptions)){
        search_folder(folder);}
        cout<<"\033[1;35m"+folder+"\033[0m"<<endl;
    } }
    catch(...){
        ;
    }}
}
int main(){
    time_t start = time(nullptr);
    thread t1 = thread(checker);
    t1.detach();
    thread t2 = thread(search_folder, path);
    t2.join();
    active = false;
    cout<<"\n\n\n\033[1;31mResults:\033[0m\n";
    for(const string element: results){
        cout<<"\033[1;32m    "+element+"\033[0m"<<endl;
    }
    cout<<"\n";
    cout<<"\033[1;31mEnded searching "+to_string(num)+" files "+to_string(time(nullptr)-start)+"secs\033[0m\n";
}
