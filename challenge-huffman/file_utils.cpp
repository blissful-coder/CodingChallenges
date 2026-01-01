#include "file_utils.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <map>

namespace FileUtils
{
    std::string readFileForParsing(const std::string &filename)
    {
        std::ifstream rfile(filename, std::ios::binary);
        if (!rfile.is_open())
            throw std::runtime_error("Could not open file: " + filename);

        rfile.seekg(0, std::ios::end);
        std::streampos pos = rfile.tellg();
        if (pos < 0)
            throw std::runtime_error("Failed to determine file size: " + filename);

        std::size_t size = static_cast<std::size_t>(pos);
        rfile.seekg(0, std::ios::beg);
        if (size == 0)
            return {};

        std::string content(size, '\0');
        rfile.read(&content[0], size);
        if (rfile.fail() || static_cast<std::size_t>(rfile.gcount()) != size)
            throw std::runtime_error("Failed to read complete file: " + filename);

        return content;
    }

    void printUsage()
    {
        std::cout << "\nUsage: ./huff <input_file> [output_file]\n";
        std::cout << "Example: ./huff test.txt compressed.huf\n";
        std::cout << "If output_file is not specified, uses input_file.huf\n";
    }

    void writeCompressedFile(const std::string &outputFilename,
                           const std::map<char, int> &frequencies,
                           const std::string &compressedData)
    {
        std::ofstream outFile(outputFilename, std::ios::binary);
        if (!outFile.is_open())
            throw std::runtime_error("Could not create output file: " + outputFilename);

        // Write header: frequency table
        // Format: [num_chars][char1][freq1][char2][freq2]...[DELIMITER]
        
        // Write number of unique characters (4 bytes)
        uint32_t numChars = static_cast<uint32_t>(frequencies.size());
        outFile.write(reinterpret_cast<const char*>(&numChars), sizeof(numChars));
        
        // Write each character and its frequency
        for (const auto &[ch, freq] : frequencies)
        {
            // Write character (1 byte)
            outFile.write(&ch, 1);
            // Write frequency (4 bytes)
            uint32_t frequency = static_cast<uint32_t>(freq);
            outFile.write(reinterpret_cast<const char*>(&frequency), sizeof(frequency));
        }
        
        // Write delimiter to separate header from data
        const char delimiter[] = "HUFFDATA";
        outFile.write(delimiter, 8);
        
        // Write compressed data
        outFile.write(compressedData.c_str(), compressedData.length());
        
        if (outFile.fail())
            throw std::runtime_error("Failed to write compressed file: " + outputFilename);
        
        std::cout << "Successfully wrote compressed file: " << outputFilename << std::endl;
        std::cout << "Header size: " << (4 + frequencies.size() * 5 + 8) << " bytes" << std::endl;
        std::cout << "Compressed data size: " << compressedData.length() << " bytes" << std::endl;
    }

    std::map<char, int> readFrequencyHeader(const std::string &filename)
    {
        std::ifstream inFile(filename, std::ios::binary);
        if (!inFile.is_open())
            throw std::runtime_error("Could not open compressed file: " + filename);

        std::map<char, int> frequencies;
        
        // Read number of unique characters
        uint32_t numChars;
        inFile.read(reinterpret_cast<char*>(&numChars), sizeof(numChars));
        if (inFile.fail())
            throw std::runtime_error("Failed to read header from: " + filename);
        
        // Read each character and frequency
        for (uint32_t i = 0; i < numChars; ++i)
        {
            char ch;
            uint32_t freq;
            
            inFile.read(&ch, 1);
            inFile.read(reinterpret_cast<char*>(&freq), sizeof(freq));
            
            if (inFile.fail())
                throw std::runtime_error("Failed to read frequency data from: " + filename);
            
            frequencies[ch] = static_cast<int>(freq);
        }
        
        // Verify delimiter
        char delimiter[9] = {0};
        inFile.read(delimiter, 8);
        if (inFile.fail() || std::string(delimiter) != "HUFFDATA")
            throw std::runtime_error("Invalid or corrupted compressed file: " + filename);
        
        return frequencies;
    }
}
