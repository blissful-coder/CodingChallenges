#include "parser.h"

Parser::Parser(Lexer l) : lex(std::move(l)) 
{ 
    advance(); 
}

void Parser::advance() 
{ 
    cur = lex.next(); 
}

bool Parser::parse()
{
    if (cur.type != TokenType::LBRACE)
        return false;
    advance();

    if (cur.type == TokenType::RBRACE)
    { // empty object
        advance();
        return cur.type == TokenType::EOF_TOKEN;
    }

    // Parse first key-value pair
    if (!parseKeyValue())
        return false;

    // Parse additional key-value pairs
    while (cur.type == TokenType::COMMA)
    {
        advance(); // consume comma

        // Check for trailing comma (invalid)
        if (cur.type == TokenType::RBRACE)
            return false;

        if (!parseKeyValue())
            return false;
    }

    if (cur.type != TokenType::RBRACE)
        return false;
    advance();

    return cur.type == TokenType::EOF_TOKEN;
}

bool Parser::parseKeyValue()
{
    // Parse key (must be string)
    if (cur.type != TokenType::STRING)
        return false;
    advance();

    // Parse colon
    if (cur.type != TokenType::COLON)
        return false;
    advance();

    // Parse value
    if (!parseValue())
        return false;

    return true;
}

bool Parser::parseValue()
{
    if (cur.type == TokenType::TRUE){
        advance();
        return true;
    }      

    if (cur.type == TokenType::FALSE){
        advance();
        return true;
    }

    if (cur.type == TokenType::NULL_TOKEN){
        advance();
        return true;
    }

    if (cur.type == TokenType::NUMBER){
        advance();
        return true;
    }
    
    if (cur.type == TokenType::STRING){
        advance();
        return true;
    }

    // Parse nested object
    if (cur.type == TokenType::LBRACE){
        return parseObject();
    }

    // Parse array
    if (cur.type == TokenType::LBRACKET){
        return parseArray();
    }

    return false;
}

bool Parser::parseObject()
{
    if (cur.type != TokenType::LBRACE)
        return false;
    advance();

    if (cur.type == TokenType::RBRACE)
    { // empty object
        advance();
        return true;
    }

    // Parse first key-value pair
    if (!parseKeyValue())
        return false;

    // Parse additional key-value pairs
    while (cur.type == TokenType::COMMA)
    {
        advance(); // consume comma

        // Check for trailing comma (invalid)
        if (cur.type == TokenType::RBRACE)
            return false;

        if (!parseKeyValue())
            return false;
    }

    if (cur.type != TokenType::RBRACE)
        return false;
    advance();

    return true;
}

bool Parser::parseArray()
{
    if (cur.type != TokenType::LBRACKET)
        return false;
    advance();

    if (cur.type == TokenType::RBRACKET)
    { // empty array
        advance();
        return true;
    }

    // Parse first value
    if (!parseValue())
        return false;

    // Parse additional values
    while (cur.type == TokenType::COMMA)
    {
        advance(); // consume comma

        // Check for trailing comma (invalid)
        if (cur.type == TokenType::RBRACKET)
            return false;

        if (!parseValue())
            return false;
    }

    if (cur.type != TokenType::RBRACKET)
        return false;
    advance();

    return true;
}
