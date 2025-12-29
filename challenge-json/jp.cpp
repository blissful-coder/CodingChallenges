#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>

std::string readFileForParsing(const std::string& filename);
void printUsage();

int main(int argc, char** argv) {
    if (argc != 2) {
        printUsage();
        return 1;
    }
    try {
        // Read JSON file
        std::string fileContent = readFileForParsing(argv[1]);
        std::cout << "\nRAW STRING: " << fileContent << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

std::string readFileForParsing(const std::string& filename) {
    std::ifstream rfile(filename, std::ios::binary);
    
    if (!rfile.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    
    // Read entire file preserving exact content
    rfile.seekg(0, std::ios::end);
    std::streampos pos = rfile.tellg();
    
    // Check if tellg() failed
    if (pos < 0) {
        throw std::runtime_error("Failed to determine file size: " + filename);
    }
    
    std::size_t size = static_cast<std::size_t>(pos);
    rfile.seekg(0, std::ios::beg);
    
    // Handle empty files safely
    if (size == 0) {
        return {};
    }
    
    std::string content(size, '\0');
    rfile.read(&content[0], size);
    
    // Verify the read was successful and complete
    if (rfile.fail() || static_cast<std::size_t>(rfile.gcount()) != size) {
        throw std::runtime_error("Failed to read complete file: " + filename);
    }
    
    return content;
}

void printUsage() {
    std::cout << "\nUsage: ./jp <file_name>" << std::endl;
    std::cout << "Example: ./jp test.json" << std::endl;
}
