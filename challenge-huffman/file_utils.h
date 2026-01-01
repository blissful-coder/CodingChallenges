#pragma once

#include <string>

namespace FileUtils
{
    std::string readFileForParsing(const std::string &filename);
    void printUsage();
}
