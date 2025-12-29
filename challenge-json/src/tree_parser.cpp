#include "tree_parser.h"
#include <iostream>

TreeParser::TreeParser(Lexer l) : lex(std::move(l)), cur(TokenType::INVALID, 0)
{ 
    advance(); 
}

void TreeParser::advance() 
{ 
    cur = lex.next(); 
}

ParseTreePtr TreeParser::parse()
{
    if (cur.type != TokenType::LBRACE)
        return nullptr;
    
    return parseObject();
}

ParseTreePtr TreeParser::parseKeyValue(const std::string& key)
{
    // Parse colon
    if (cur.type != TokenType::COLON)
        return nullptr;
    advance();

    // Parse value
    ParseTreePtr value = parseValue();
    if (value) {
        value->key = key;
    }
    return value;
}

ParseTreePtr TreeParser::parseValue()
{
    if (cur.type == TokenType::TRUE) {
        advance();
        return std::make_shared<ParseTreeNode>(NodeType::BOOLEAN, "true");
    }

    if (cur.type == TokenType::FALSE) {
        advance();
        return std::make_shared<ParseTreeNode>(NodeType::BOOLEAN, "false");
    }

    if (cur.type == TokenType::NULL_TOKEN) {
        advance();
        return std::make_shared<ParseTreeNode>(NodeType::NULL_VALUE, "null");
    }

    if (cur.type == TokenType::NUMBER) {
        std::string value = cur.value;
        advance();
        return std::make_shared<ParseTreeNode>(NodeType::NUMBER, value);
    }
    
    if (cur.type == TokenType::STRING) {
        std::string value = cur.value;
        advance();
        return std::make_shared<ParseTreeNode>(NodeType::STRING, value);
    }

    // Parse nested object
    if (cur.type == TokenType::LBRACE) {
        return parseObject();
    }

    // Parse array
    if (cur.type == TokenType::LBRACKET) {
        return parseArray();
    }

    return nullptr;
}

ParseTreePtr TreeParser::parseObject()
{
    if (cur.type != TokenType::LBRACE)
        return nullptr;
    advance();

    auto objNode = std::make_shared<ParseTreeNode>(NodeType::OBJECT);

    if (cur.type == TokenType::RBRACE) {
        // empty object
        advance();
        return objNode;
    }

    // Parse first key-value pair
    if (cur.type != TokenType::STRING)
        return nullptr;
    
    std::string key = cur.value;
    advance();
    
    ParseTreePtr value = parseKeyValue(key);
    if (!value)
        return nullptr;
    
    objNode->addChild(key, value);

    // Parse additional key-value pairs
    while (cur.type == TokenType::COMMA) {
        advance(); // consume comma

        // Check for trailing comma (invalid)
        if (cur.type == TokenType::RBRACE)
            return nullptr;

        if (cur.type != TokenType::STRING)
            return nullptr;
        
        key = cur.value;
        advance();
        
        value = parseKeyValue(key);
        if (!value)
            return nullptr;
        
        objNode->addChild(key, value);
    }

    if (cur.type != TokenType::RBRACE)
        return nullptr;
    advance();

    return objNode;
}

ParseTreePtr TreeParser::parseArray()
{
    if (cur.type != TokenType::LBRACKET)
        return nullptr;
    advance();

    auto arrayNode = std::make_shared<ParseTreeNode>(NodeType::ARRAY);

    if (cur.type == TokenType::RBRACKET) {
        // empty array
        advance();
        return arrayNode;
    }

    // Parse first value
    ParseTreePtr value = parseValue();
    if (!value)
        return nullptr;
    
    arrayNode->addArrayItem(value);

    // Parse additional values
    while (cur.type == TokenType::COMMA) {
        advance(); // consume comma

        // Check for trailing comma (invalid)
        if (cur.type == TokenType::RBRACKET)
            return nullptr;

        value = parseValue();
        if (!value)
            return nullptr;
        
        arrayNode->addArrayItem(value);
    }

    if (cur.type != TokenType::RBRACKET)
        return nullptr;
    advance();

    return arrayNode;
}
