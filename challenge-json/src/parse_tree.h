#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

enum class NodeType {
    OBJECT,
    ARRAY,
    STRING,
    NUMBER,
    BOOLEAN,
    NULL_VALUE
};

class ParseTreeNode {
public:
    NodeType type;
    std::string value;
    std::string key;  // For object members
    
    // For object nodes
    std::unordered_map<std::string, std::shared_ptr<ParseTreeNode>> children;
    
    // For array nodes
    std::vector<std::shared_ptr<ParseTreeNode>> arrayItems;
    
    ParseTreeNode(NodeType t, const std::string& val = "", const std::string& k = "");
    
    // Add child to object
    void addChild(const std::string& key, std::shared_ptr<ParseTreeNode> child);
    
    // Add item to array
    void addArrayItem(std::shared_ptr<ParseTreeNode> item);
    
    // Helper methods for display
    std::string getDisplayValue() const;
    std::string getNodeTypeString() const;
};

using ParseTreePtr = std::shared_ptr<ParseTreeNode>;
