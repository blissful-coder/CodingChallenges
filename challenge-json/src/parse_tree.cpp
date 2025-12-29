#include "parse_tree.h"

ParseTreeNode::ParseTreeNode(NodeType t, const std::string& val, const std::string& k) 
    : type(t), value(val), key(k) {}

void ParseTreeNode::addChild(const std::string& key, std::shared_ptr<ParseTreeNode> child) {
    children[key] = child;
}

void ParseTreeNode::addArrayItem(std::shared_ptr<ParseTreeNode> item) {
    arrayItems.push_back(item);
}

std::string ParseTreeNode::getDisplayValue() const {
    switch (type) {
        case NodeType::OBJECT:
            return "Object (" + std::to_string(children.size()) + " members)";
        case NodeType::ARRAY:
            return "Array (" + std::to_string(arrayItems.size()) + " items)";
        case NodeType::STRING:
            return "\"" + value + "\"";
        case NodeType::NUMBER:
            return value;
        case NodeType::BOOLEAN:
            return value;
        case NodeType::NULL_VALUE:
            return "null";
        default:
            return "Unknown";
    }
}

std::string ParseTreeNode::getNodeTypeString() const {
    switch (type) {
        case NodeType::OBJECT: return "Object";
        case NodeType::ARRAY: return "Array";
        case NodeType::STRING: return "String";
        case NodeType::NUMBER: return "Number";
        case NodeType::BOOLEAN: return "Boolean";
        case NodeType::NULL_VALUE: return "Null";
        default: return "Unknown";
    }
}
