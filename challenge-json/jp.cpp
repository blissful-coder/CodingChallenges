#include <iostream>
#include <fstream>
#include <map>

#define sz 4
using namespace std;

int simpleDFA [sz][sz] = {
    {0,1,3,3},
    {1,3,2,3},
    {2,3,3,3},
    {3,3,3,3}
};


int mapper(char key){
    map<char,int> mapper;
    
    mapper[' ']= 0;
    mapper['{']= 1;
    mapper['}']= 2;

    auto it = mapper.find(key);
    if (it == mapper.end()) {
        return 3;
    }
    
    return mapper[key];
}

int theJsonParser(std::string input){
    int current_state = 0;
    int final_state = 2;
    for (int i=0; i<input.size(); i++){
        current_state = simpleDFA[current_state][mapper(input[i])];
        cout<<current_state;
    }
    if(current_state == final_state)
        return 0;

    return 1;
}

int main(int argc, char *argv[]){

    if(theJsonParser(argv[1])==0){
        cout<<"\nValid JSON"<<endl;
    }
    else{
        cout<<"\nInvalid JSON"<<endl;
    }
    return 0;
}