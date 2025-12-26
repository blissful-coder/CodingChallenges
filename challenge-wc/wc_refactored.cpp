#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <cctype>
#include <algorithm>

class WordCounter {
private:
    std::string content_;
    bool valid_;

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            valid_ = false;
            return;
        }
        
        std::ostringstream buffer;
        buffer << file.rdbuf();
        content_ = buffer.str();
        valid_ = true;
    }

    void loadFromStdin() {
        std::ostringstream buffer;
        std::string line;
        while (std::getline(std::cin, line)) {
            buffer << line << '\n';
        }
        content_ = buffer.str();
        valid_ = true;
    }

public:
    explicit WordCounter(const std::string& filename) {
        loadFromFile(filename);
    }

    WordCounter() {
        loadFromStdin();
    }

    bool isValid() const { return valid_; }

    size_t countLines() const {
        if (!valid_ || content_.empty()) return 0;
        
        size_t lines = 0;
        for (char ch : content_) {
            if (ch == '\n') {
                ++lines;
            }
        }
        return lines;
    }

    size_t countWords() const {
        if (!valid_ || content_.empty()) return 0;
        
        size_t words = 0;
        bool inWord = false;
        
        for (char ch : content_) {
            if (std::isspace(ch)) {
                inWord = false;
            } else if (!inWord) {
                inWord = true;
                ++words;
            }
        }
        return words;
    }

    size_t countBytes() const {
        return valid_ ? content_.size() : 0;
    }

    size_t countCharacters() const {
        // For ASCII text, character count equals byte count
        // For proper UTF-8 support, would need additional logic
        return countBytes();
    }
};

class CommandLineParser {
private:
    std::vector<std::string> args_;
    
public:
    explicit CommandLineParser(int argc, char* argv[]) {
        for (int i = 1; i < argc; ++i) {
            args_.emplace_back(argv[i]);
        }
    }

    bool hasOption(const std::string& option) const {
        return std::find(args_.begin(), args_.end(), option) != args_.end();
    }

    std::string getFilename() const {
        // Return the first argument that doesn't start with '-'
        for (const auto& arg : args_) {
            if (!arg.empty() && arg[0] != '-') {
                return arg;
            }
        }
        return "";
    }

    bool isValidOption(const std::string& option) const {
        return option == "-l" || option == "-w" || option == "-c" || option == "-m";
    }

    bool hasValidOptions() const {
        for (const auto& arg : args_) {
            if (!arg.empty() && arg[0] == '-' && !isValidOption(arg)) {
                return false;
            }
        }
        return true;
    }

    size_t getArgCount() const {
        return args_.size();
    }

    void printUsage(const std::string& programName) const {
        std::cerr << "Usage: " << programName << " <-l | -w | -c | -m> [filename]" << std::endl;
    }
};

void printResults(const WordCounter& counter, const CommandLineParser& parser, const std::string& filename) {
    if (parser.hasOption("-l")) {
        std::cout << counter.countLines();
        if (!filename.empty()) {
            std::cout << " " << filename;
        }
        std::cout << std::endl;
    } else if (parser.hasOption("-c")) {
        std::cout << counter.countBytes();
        if (!filename.empty()) {
            std::cout << " " << filename;
        }
        std::cout << std::endl;
    } else if (parser.hasOption("-m")) {
        std::cout << counter.countCharacters();
        if (!filename.empty()) {
            std::cout << " " << filename;
        }
        std::cout << std::endl;
    } else if (parser.hasOption("-w")) {
        std::cout << counter.countWords();
        if (!filename.empty()) {
            std::cout << " " << filename;
        }
        std::cout << std::endl;
    } else {
        // Default behavior: show lines, words, bytes
        std::cout << counter.countLines() << " " 
                  << counter.countWords() << " " 
                  << counter.countBytes();
        if (!filename.empty()) {
            std::cout << " " << filename;
        }
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[]) {
    CommandLineParser parser(argc, argv);

    // Validate arguments
    if (argc < 1 || argc > 3) {
        parser.printUsage(argv[0]);
        return 1;
    }

    if (!parser.hasValidOptions()) {
        parser.printUsage(argv[0]);
        return 1;
    }

    std::string filename = parser.getFilename();
    std::unique_ptr<WordCounter> counter;

    // Handle different input scenarios
    if (argc == 2 && !std::cin.peek() && filename.empty()) {
        // Single option with no file and no stdin
        parser.printUsage(argv[0]);
        return 1;
    } else if (filename.empty()) {
        // Read from stdin
        counter = std::make_unique<WordCounter>();
    } else {
        // Read from file
        counter = std::make_unique<WordCounter>(filename);
    }

    if (!counter->isValid()) {
        return 1;
    }

    printResults(*counter, parser, filename);
    return 0;
}
