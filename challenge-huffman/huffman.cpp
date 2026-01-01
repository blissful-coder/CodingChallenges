#include "file_utils.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <map>
#include <cstddef>
#include <cctype>
#include <iomanip>
#include <string>
#include <unordered_map>
#include <sstream>
#include <memory>
#include <queue>

struct Node
{
    char ch;
    int freq;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;

    Node(char c, int f) : ch(c), freq(f) {}
    Node(char c, int f, std::shared_ptr<Node> l, std::shared_ptr<Node> r)
        : ch(c), freq(f), left(std::move(l)), right(std::move(r)) {}
    bool isLeaf() const { return !left && !right; }
};

struct Compare
{
    bool operator()(const std::shared_ptr<Node> &a, const std::shared_ptr<Node> &b)
        const
    {
        return a->freq > b->freq; // min-heap
    }
};

using MinHeap = std::priority_queue<
    std::shared_ptr<Node>,
    std::vector<std::shared_ptr<Node>>,
    Compare>;

void frequencyMap(const std::string &book, std::map<char, int> &fillThis);
void printFrequency(const std::map<char, int> &freq);
std::shared_ptr<Node> buildHuffmanTree(const std::map<char, int> &fmap);
void buildCodes(const std::shared_ptr<Node>& node,
                const std::string& path,
                std::unordered_map<char, std::string>& codes);
std::string compressText(const std::string &text, 
                        const std::unordered_map<char, std::string> &codes);
std::string generateOutputFilename(const std::string &inputFilename);
std::string decompressText(const std::string &compressedBits, 
                          const std::shared_ptr<Node> &root);
bool isCompressedFile(const std::string &filename);

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 3)
    {
        FileUtils::printUsage();
        return 1;
    }
    
    try
    {
        std::string inputFilename = argv[1];
        std::string outputFilename;
        
        if (argc == 3)
        {
            outputFilename = argv[2];
        }
        
        // Check if input file is a compressed file
        if (isCompressedFile(inputFilename))
        {
            // DECOMPRESSION MODE
            std::cout << "Decompression mode detected.\n";
            
            if (argc == 2)
            {
                // Generate output filename by removing .huf extension
                size_t lastDot = inputFilename.find_last_of('.');
                if (lastDot != std::string::npos && inputFilename.substr(lastDot) == ".huf")
                {
                    outputFilename = inputFilename.substr(0, lastDot) + "_decompressed.txt";
                }
                else
                {
                    outputFilename = inputFilename + "_decompressed.txt";
                }
            }
            
            // Step 6: Read header and rebuild frequency map
            std::map<char, int> frequencies = FileUtils::readFrequencyHeader(inputFilename);
            
            // Rebuild Huffman tree from frequencies
            auto root = buildHuffmanTree(frequencies);
            
            // Step 7: Read compressed data and decompress
            std::string compressedBits = FileUtils::readCompressedData(inputFilename);
            std::string decompressedText = decompressText(compressedBits, root);
            
            // Write decompressed text to output file
            std::ofstream outFile(outputFilename);
            if (!outFile.is_open())
                throw std::runtime_error("Could not create output file: " + outputFilename);
            
            outFile.write(decompressedText.c_str(), decompressedText.length());
            if (outFile.fail())
                throw std::runtime_error("Failed to write decompressed file: " + outputFilename);
            
            std::cout << "Successfully decompressed to: " << outputFilename << std::endl;
            std::cout << "Decompressed size: " << decompressedText.length() << " bytes" << std::endl;
        }
        else
        {
            // COMPRESSION MODE
            std::cout << "Compression mode detected.\n";
            
            if (argc == 2)
            {
                outputFilename = generateOutputFilename(inputFilename);
            }
            
            // Read input file
            std::string fileContent = FileUtils::readFileForParsing(inputFilename);
            
            if (fileContent.empty())
            {
                std::cout << "Input file is empty. Nothing to compress.\n";
                return 0;
            }
            
            // Build frequency map
            std::map<char, int> wordFrequency;
            frequencyMap(fileContent, wordFrequency);
            
            // Build Huffman tree and generate codes
            auto root = buildHuffmanTree(wordFrequency);
            std::unordered_map<char, std::string> codes;
            buildCodes(root, "", codes);

            // Display the generated codes
            std::cout << "\n--- HUFFMAN CODES ---\n";
            for (const auto &[ch, code] : codes)
            {
                if (std::isprint(static_cast<unsigned char>(ch)))
                    std::cout << "'" << ch << "' -> " << code << "\n";
                else
                    std::cout << "0x" << std::hex
                              << (static_cast<unsigned char>(ch) & 0xFF)
                              << std::dec << " -> " << code << "\n";
            }
            
            // Compress the text
            std::string compressedData = compressText(fileContent, codes);
            
            // Write compressed file with header
            FileUtils::writeCompressedFile(outputFilename, wordFrequency, compressedData);
            
            // Display compression statistics
            std::cout << "\n--- COMPRESSION STATISTICS ---\n";
            std::cout << "Original size: " << fileContent.size() << " bytes (" 
                      << (fileContent.size() * 8) << " bits)\n";
            std::cout << "Compressed bits: " << compressedData.length() << " bits\n";
            std::cout << "Packed size: " << ((compressedData.length() + 7) / 8) << " bytes\n";
            
            if (!fileContent.empty())
            {
                double compressionRatio = static_cast<double>(compressedData.length()) / 
                                         (fileContent.size() * 8) * 100.0;
                std::cout << "Compression ratio: " << std::fixed << std::setprecision(2) 
                          << compressionRatio << "%\n";
                std::cout << "Space saved: " << std::fixed << std::setprecision(2)
                          << (100.0 - compressionRatio) << "%\n";
            }
        }
        
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}

void frequencyMap(const std::string &book, std::map<char, int> &fillThis)
{
    for (std::size_t i = 0; i < book.size(); ++i)
    {
        if (fillThis.find(book[i]) != fillThis.end())
        {
            fillThis[book[i]] = fillThis[book[i]] + 1;
        }
        else
        {
            fillThis[book[i]] = 1;
        }
    }
}

void printFrequency(const std::map<char, int> &freq)
{
    std::cout << "\n--- CHARACTER FREQUENCY ANALYSIS ---\n";

    for (const auto &pair : freq)
    {
        char ch = pair.first;
        int count = pair.second;

        // Handle printable ASCII characters
        if (std::isprint(static_cast<unsigned char>(ch)))
        {
            std::cout << "'" << ch << "': " << count << std::endl;
        }
        // Handle common whitespace characters
        else if (ch == '\n')
        {
            std::cout << "'\\n' (newline): " << count << std::endl;
        }
        else if (ch == '\r')
        {
            std::cout << "'\\r' (carriage return): " << count << std::endl;
        }
        else if (ch == '\t')
        {
            std::cout << "'\\t' (tab): " << count << std::endl;
        }
        else if (ch == ' ')
        {
            std::cout << "' ' (space): " << count << std::endl;
        }
        // Handle non-printable characters (including UTF-8 bytes)
        else
        {
            std::cout << "0x" << std::hex << std::setfill('0') << std::setw(2)
                      << (static_cast<unsigned char>(ch) & 0xFF) << std::dec
                      << " (non-printable/UTF-8 byte): " << count << std::endl;
        }
    }
}

std::shared_ptr<Node> buildHuffmanTree(const std::map<char, int> &fmap)
{
    MinHeap huffHeap;

    for (const auto &[ch, count] : fmap)
    {
        huffHeap.push(std::make_shared<Node>(ch, count));
    }
    if (huffHeap.empty())
        return nullptr;
    if (huffHeap.size() == 1)
    {
        // Common trick: make a parent so codes still exist
        auto only = huffHeap.top();
        huffHeap.pop();
        return std::make_shared<Node>('\0', only->freq, only, nullptr);
    }
    while (huffHeap.size() > 1)
    {
        auto left = huffHeap.top();
        huffHeap.pop();
        auto right = huffHeap.top();
        huffHeap.pop();

        auto merged = std::make_shared<Node>(
            '\0', left->freq + right->freq, left, right);
        huffHeap.push(merged);
    }
    return huffHeap.top(); // root
}

void buildCodes(const std::shared_ptr<Node>& node,
                const std::string& path,
                std::unordered_map<char, std::string>& codes) {
    if (!node) return;

    if (node->isLeaf()) {
        // If only one symbol existed, path could be empty: assign "0"
        codes[node->ch] = path.empty() ? "0" : path;
        return;
    }

    buildCodes(node->left,  path + "0", codes);
    buildCodes(node->right, path + "1", codes);
}

std::string compressText(const std::string &text, 
                        const std::unordered_map<char, std::string> &codes)
{
    std::ostringstream compressed;
    
    for (char ch : text)
    {
        auto it = codes.find(ch);
        if (it != codes.end())
        {
            compressed << it->second;
        }
        else
        {
            throw std::runtime_error("Character not found in Huffman codes: " + std::to_string(static_cast<unsigned char>(ch)));
        }
    }
    
    return compressed.str();
}

std::string generateOutputFilename(const std::string &inputFilename)
{
    size_t lastDot = inputFilename.find_last_of('.');
    if (lastDot != std::string::npos)
    {
        return inputFilename.substr(0, lastDot) + ".huf";
    }
    return inputFilename + ".huf";
}

std::string decompressText(const std::string &compressedBits, 
                          const std::shared_ptr<Node> &root)
{
    if (!root || compressedBits.empty())
        return "";
    
    std::string result;
    auto current = root;
    
    for (char bit : compressedBits)
    {
        // Traverse tree based on bit value
        if (bit == '0')
        {
            current = current->left;
        }
        else if (bit == '1')
        {
            current = current->right;
        }
        else
        {
            throw std::runtime_error("Invalid bit in compressed data: " + std::to_string(bit));
        }
        
        // If we reach a leaf node, we've found a character
        if (current && current->isLeaf())
        {
            result += current->ch;
            current = root;  // Reset to root for next character
        }
        
        // Safety check - if current is null, something went wrong
        if (!current)
        {
            throw std::runtime_error("Invalid path in Huffman tree during decompression");
        }
    }
    
    return result;
}

bool isCompressedFile(const std::string &filename)
{
    try
    {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open())
            return false;
        
        // Read first 4 bytes to get number of characters
        uint32_t numChars;
        file.read(reinterpret_cast<char*>(&numChars), sizeof(numChars));
        if (file.fail())
            return false;
        
        // Skip frequency data
        file.seekg(numChars * 5, std::ios::cur);
        
        // Check for delimiter
        char delimiter[9] = {0};
        file.read(delimiter, 8);
        if (file.fail())
            return false;
        
        return std::string(delimiter) == "HUFFDATA";
    }
    catch (...)
    {
        return false;
    }
}
