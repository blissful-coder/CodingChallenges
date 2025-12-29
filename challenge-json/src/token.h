#pragma once

#include <cstddef>
#include <string>

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
    std::string value; // Store the actual token value
    char badChar; // only meaningful for INVALID
    
    Token(TokenType t, std::size_t p, const std::string& val = "", char bc = '\0')
        : type(t), pos(p), value(val), badChar(bc) {}
};

const char* tokenName(TokenType t);
