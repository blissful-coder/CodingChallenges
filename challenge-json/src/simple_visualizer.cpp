#include "simple_visualizer.h"
#include "tree_parser.h"
#include "lexer.h"
#include <iostream>
#include <iomanip>

void SimpleVisualizer::printTree(ParseTreePtr node, int depth)
{
    if (!node) return;
    
    std::string indent = getIndent(depth);
    std::string nodeInfo = node->key.empty() ? "Root" : node->key;
    
    std::cout << indent << "├─ " << nodeInfo 
              << " [" << node->getNodeTypeString() << "] "
              << node->getDisplayValue() << std::endl;
    
    // Print object children
    if (node->type == NodeType::OBJECT) {
        for (const auto& pair : node->children) {
            printTree(pair.second, depth + 1);
        }
    }
    
    // Print array items
    if (node->type == NodeType::ARRAY) {
        for (size_t i = 0; i < node->arrayItems.size(); ++i) {
            auto child = node->arrayItems[i];
            std::string arrayIndent = getIndent(depth + 1);
            std::cout << arrayIndent << "├─ [" << i << "] "
                      << "[" << child->getNodeTypeString() << "] "
                      << child->getDisplayValue() << std::endl;
            
            // If array item is object or array, print its children
            if (child->type == NodeType::OBJECT) {
                for (const auto& pair : child->children) {
                    printTree(pair.second, depth + 2);
                }
            } else if (child->type == NodeType::ARRAY) {
                printTree(child, depth + 1);
            }
        }
    }
}

std::string SimpleVisualizer::parseJsonToString(const std::string& json)
{
    std::stringstream result;
    
    try {
        // Disable lexer output
        std::cout.setstate(std::ios_base::badbit);
        
        Lexer lexer(json);
        TreeParser parser(std::move(lexer));
        
        ParseTreePtr root = parser.parse();
        
        // Re-enable cout
        std::cout.clear();
        
        if (root) {
            result << "JSON Parse Tree:\n";
            result << "================\n\n";
            printNodeToStream(root, result, 0);
            result << "\nParsing completed successfully!\n";
        } else {
            result << "Failed to parse JSON - Invalid syntax\n";
        }
    } catch (const std::exception& e) {
        std::cout.clear();
        result << "Parse error: " << e.what() << "\n";
    }
    
    return result.str();
}

std::string SimpleVisualizer::getIndent(int depth)
{
    std::string indent;
    for (int i = 0; i < depth; ++i) {
        indent += "  ";
    }
    return indent;
}

void SimpleVisualizer::printNodeToStream(ParseTreePtr node, std::stringstream& ss, int depth)
{
    if (!node) return;
    
    std::string indent = getIndent(depth);
    std::string nodeInfo = node->key.empty() ? "Root" : node->key;
    
    ss << indent << "├─ " << nodeInfo 
       << " [" << node->getNodeTypeString() << "] "
       << node->getDisplayValue() << "\n";
    
    // Print object children
    if (node->type == NodeType::OBJECT) {
        for (const auto& pair : node->children) {
            printNodeToStream(pair.second, ss, depth + 1);
        }
    }
    
    // Print array items
    if (node->type == NodeType::ARRAY) {
        for (size_t i = 0; i < node->arrayItems.size(); ++i) {
            auto child = node->arrayItems[i];
            std::string arrayIndent = getIndent(depth + 1);
            ss << arrayIndent << "├─ [" << i << "] "
               << "[" << child->getNodeTypeString() << "] "
               << child->getDisplayValue() << "\n";
            
            // If array item is object or array, print its children
            if (child->type == NodeType::OBJECT) {
                for (const auto& pair : child->children) {
                    printNodeToStream(pair.second, ss, depth + 2);
                }
            } else if (child->type == NodeType::ARRAY) {
                printNodeToStream(child, ss, depth + 1);
            }
        }
    }
}
