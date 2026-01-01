#include "file_utils.h"
#include <iostream>
#include <stdexcept>
#include <map>
#include <cstddef>
#include <cctype>
#include <iomanip>

void frequencyMap(const std::string& book, std::map<char, int>& fillThis);
void printFrequency(const std::map<char, int>& freq);

int main(int argc, char **argv)
{
    std::map<char, int> wordFrequency;
    if (argc != 2)
    {
        FileUtils::printUsage();
        return 1;
    }
    try
    {
        std::string fileContent = FileUtils::readFileForParsing(argv[1]);
        frequencyMap(fileContent, wordFrequency);
        printFrequency(wordFrequency);
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}

void frequencyMap(const std::string& book, std::map<char, int>& fillThis)
{
    for (std::size_t i = 0; i < book.size(); ++i) {
        if (fillThis.find(book[i]) != fillThis.end()) {
            fillThis[book[i]] = fillThis[book[i]] + 1;
        } else {
            fillThis[book[i]] = 1;
        }
    }
}

void printFrequency(const std::map<char, int>& freq) {
    std::cout << "\n--- CHARACTER FREQUENCY ANALYSIS ---\n";
    
    for (const auto& pair : freq) {
        char ch = pair.first;
        int count = pair.second;
        
        // Handle printable ASCII characters
        if (std::isprint(static_cast<unsigned char>(ch))) {
            std::cout << "'" << ch << "': " << count << std::endl;
        }
        // Handle common whitespace characters
        else if (ch == '\n') {
            std::cout << "'\\n' (newline): " << count << std::endl;
        }
        else if (ch == '\r') {
            std::cout << "'\\r' (carriage return): " << count << std::endl;
        }
        else if (ch == '\t') {
            std::cout << "'\\t' (tab): " << count << std::endl;
        }
        else if (ch == ' ') {
            std::cout << "' ' (space): " << count << std::endl;
        }
        // Handle non-printable characters (including UTF-8 bytes)
        else {
            std::cout << "0x" << std::hex << std::setfill('0') << std::setw(2) 
                      << (static_cast<unsigned char>(ch) & 0xFF) << std::dec 
                      << " (non-printable/UTF-8 byte): " << count << std::endl;
        }
    }
}
