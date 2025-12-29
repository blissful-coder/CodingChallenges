#pragma once

#include "lexer.h"
#include "token.h"
#include "parse_tree.h"

class TreeParser
{
private:
    Lexer lex;
    Token cur;

    void advance();
    ParseTreePtr parseKeyValue(const std::string& key);
    ParseTreePtr parseValue();
    ParseTreePtr parseObject();
    ParseTreePtr parseArray();

public:
    explicit TreeParser(Lexer l);
    ParseTreePtr parse();
};
