#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "parse_tree.h"

class SimpleVisualizer
{
public:
    static void printTree(ParseTreePtr node, int depth = 0);
    static std::string parseJsonToString(const std::string& json);
    
private:
    static std::string getIndent(int depth);
    static void printNodeToStream(ParseTreePtr node, std::stringstream& ss, int depth = 0);
};
