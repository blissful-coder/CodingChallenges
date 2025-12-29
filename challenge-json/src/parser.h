#pragma once

#include "lexer.h"
#include "token.h"

class Parser
{
private:
    Lexer lex;
    Token cur;

    void advance();
    bool parseKeyValue();
    bool parseValue();
    bool parseObject();
    bool parseArray();

public:
    explicit Parser(Lexer l);
    bool parse();
};
