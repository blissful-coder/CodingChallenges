#include <iostream>
#include <fstream>
#include <map>
#include <string>

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

// Function to read file contents
string readFileContents(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file '" << filename << "'" << endl;
        return "";
    }
    
    string content;
    string line;
    while (getline(file, line)) {
        content += line;
        content += "\n"; // Preserve line breaks
    }
    file.close();
    
    // Remove the last newline if it exists
    if (!content.empty() && content.back() == '\n') {
        content.pop_back();
    }
    
    return content;
}

// Function to check if a file exists
bool fileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
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
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <input_string_or_filename>" << endl;
        cout << "  - Pass a JSON string directly: ./jp \"{}\"\n";
        cout << "  - Pass a filename: ./jp input.json\n";
        return 1;
    }
    
    string input = argv[1];
    string jsonContent;
    
    // Check if the argument is a file or a string
    if (fileExists(input)) {
        // It's a file - read the contents
        cout << "Reading from file: " << input << endl;
        jsonContent = readFileContents(input);
        if (jsonContent.empty()) {
            return 1; // Error message already printed in readFileContents
        }
    } else {
        // It's a string argument - use it directly
        cout << "Parsing input string: " << input << endl;
        jsonContent = input;
    }
    
    // Parse the JSON content
    if(theJsonParser(jsonContent) == 0){
        cout<<"\nValid JSON"<<endl;
    }
    else{
        cout<<"\nInvalid JSON"<<endl;
    }
    return 0;
}
