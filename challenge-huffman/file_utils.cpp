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
        // Format: [num_chars][char1][freq1][char2][freq2]...[DELIMITER][total_bits][packed_data]
        
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
        
        // Write total number of bits in original compressed data (for unpacking)
        uint64_t totalBits = static_cast<uint64_t>(compressedData.length());
        outFile.write(reinterpret_cast<const char*>(&totalBits), sizeof(totalBits));
        
        // Pack bits into bytes and write compressed data
        std::string packedData = packBitsToBytes(compressedData);
        outFile.write(packedData.c_str(), packedData.length());
        
        if (outFile.fail())
            throw std::runtime_error("Failed to write compressed file: " + outputFilename);
        
        std::cout << "Successfully wrote compressed file: " << outputFilename << std::endl;
        std::cout << "Header size: " << (4 + frequencies.size() * 5 + 8 + 8) << " bytes" << std::endl;
        std::cout << "Original bits: " << compressedData.length() << std::endl;
        std::cout << "Packed data size: " << packedData.length() << " bytes" << std::endl;
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

    std::string readCompressedData(const std::string &filename)
    {
        std::ifstream inFile(filename, std::ios::binary);
        if (!inFile.is_open())
            throw std::runtime_error("Could not open compressed file: " + filename);

        // Read number of unique characters
        uint32_t numChars;
        inFile.read(reinterpret_cast<char*>(&numChars), sizeof(numChars));
        if (inFile.fail())
            throw std::runtime_error("Failed to read header from: " + filename);
        
        // Skip frequency data
        inFile.seekg(numChars * 5, std::ios::cur);  // Each entry is 1 byte (char) + 4 bytes (freq)
        
        // Skip delimiter
        inFile.seekg(8, std::ios::cur);
        
        // Read total bits
        uint64_t totalBits;
        inFile.read(reinterpret_cast<char*>(&totalBits), sizeof(totalBits));
        if (inFile.fail())
            throw std::runtime_error("Failed to read total bits from: " + filename);
        
        // Read remaining data (packed bits)
        inFile.seekg(0, std::ios::end);
        std::streampos endPos = inFile.tellg();
        std::streampos currentPos = 4 + numChars * 5 + 8 + 8;  // Header size
        inFile.seekg(currentPos, std::ios::beg);
        
        size_t remainingBytes = static_cast<size_t>(endPos - currentPos);
        std::string packedData(remainingBytes, '\0');
        inFile.read(&packedData[0], remainingBytes);
        
        if (inFile.fail())
            throw std::runtime_error("Failed to read compressed data from: " + filename);
        
        // Unpack bytes back to bit string
        return unpackBytesToBits(packedData, static_cast<size_t>(totalBits));
    }

    std::string packBitsToBytes(const std::string &bitString)
    {
        std::string result;
        result.reserve((bitString.length() + 7) / 8);  // Ceiling division
        
        for (size_t i = 0; i < bitString.length(); i += 8)
        {
            unsigned char byte = 0;
            for (int j = 0; j < 8 && (i + j) < bitString.length(); ++j)
            {
                if (bitString[i + j] == '1')
                {
                    byte |= (1 << (7 - j));  // Set bit from left to right
                }
            }
            result.push_back(static_cast<char>(byte));
        }
        
        return result;
    }

    std::string unpackBytesToBits(const std::string &packedData, size_t totalBits)
    {
        std::string result;
        result.reserve(totalBits);
        
        for (size_t i = 0; i < packedData.length() && result.length() < totalBits; ++i)
        {
            unsigned char byte = static_cast<unsigned char>(packedData[i]);
            for (int j = 7; j >= 0 && result.length() < totalBits; --j)
            {
                result.push_back((byte & (1 << j)) ? '1' : '0');
            }
        }
        
        return result;
    }
}
