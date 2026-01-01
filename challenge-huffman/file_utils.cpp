#include "file_utils.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

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
        std::cout << "\nUsage: ./huff <file_name>\nExample: ./huff test.json\n";
    }
}
