#pragma once

#include "token.h"
#include <string>
#include <cstddef>

class Lexer
{
private:
    const std::string &s;
    std::size_t i = 0;

    void skipWs();
    Token parseString();
    Token parseKeyword();
    Token parseNumber();

public:
    explicit Lexer(const std::string &input);
    Token next();
};
