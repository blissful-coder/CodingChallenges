#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

std::vector<std::string> divideInParts(const std::string& line);
void printColumn(const std::vector<std::string> lineParts, const std::string optionArg);

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " -f<list> <filename>\n";
        return EXIT_FAILURE;
    }

    std::ifstream ifs(argv[2]);
    if (!ifs)
    {
        std::cerr << "Error: couldn't open file for reading\n";
        return EXIT_FAILURE;
    }

    int line_no=0;
    for(std::string line; getline(ifs,line); line_no++){
        // std::cout<<"\n"<<line;
        std::vector<std::string> lineparts = divideInParts(line);
        printColumn(lineparts,argv[1]);
    }

    return EXIT_SUCCESS;
}

std::vector<std::string> divideInParts(const std::string& line){
    std::vector<std::string> parts;
    size_t start = 0;
    
    for(size_t i = 0; i < line.size(); i++){
        if(line[i] == '\t'){
            // Extract substring from start to current position (length = i - start)
            std::string temp = line.substr(start, i - start);
            parts.push_back(temp);
            start = i + 1;
        }
    }
    
    // Don't forget the last part (after the final tab or the entire string if no tabs)
    if(start <= line.size()){
        std::string temp = line.substr(start);
        parts.push_back(temp);
    }
    
    return parts;
}

void printColumn(const std::vector<std::string> lineParts, const std::string optionArg){
    size_t col = optionArg[2] - '0' -1;
    for(size_t i=0; i<lineParts.size(); i++){
        if(i==col)
            std::cout<<lineParts[i]<<"\n";
    }
}