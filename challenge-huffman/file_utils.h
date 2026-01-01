#pragma once

#include <string>
#include <map>

namespace FileUtils
{
    std::string readFileForParsing(const std::string &filename);
    void printUsage();
    void writeCompressedFile(const std::string &outputFilename,
                           const std::map<char, int> &frequencies,
                           const std::string &compressedData);
    std::map<char, int> readFrequencyHeader(const std::string &filename);
    std::string readCompressedData(const std::string &filename);
    std::string packBitsToBytes(const std::string &bitString);
    std::string unpackBytesToBits(const std::string &packedData, size_t totalBits);
}
