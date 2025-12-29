#pragma once

#include <cstddef>

enum class TokenType
{
    LBRACE,
    RBRACE,
    LBRACKET,
    RBRACKET,
    STRING,
    COLON,
    COMMA,
    NUMBER,
    TRUE,
    FALSE,
    NULL_TOKEN,
    EOF_TOKEN,
    INVALID
};

struct Token
{
    TokenType type;
    std::size_t pos;
    char badChar; // only meaningful for INVALID
};

const char* tokenName(TokenType t);
