#include "token.h"

const char* tokenName(TokenType t)
{
    switch (t)
    {
    case TokenType::LBRACE:
        return "LBRACE";
    case TokenType::RBRACE:
        return "RBRACE";
    case TokenType::LBRACKET:
        return "LBRACKET";
    case TokenType::RBRACKET:
        return "RBRACKET";
    case TokenType::COLON:
        return "COLON";
    case TokenType::COMMA:
        return "COMMA";
    case TokenType::STRING:
        return "STRING";
    case TokenType::NUMBER:
        return "NUMBER";
    case TokenType::TRUE:
        return "TRUE";
    case TokenType::FALSE:
        return "FALSE";
    case TokenType::NULL_TOKEN:
        return "NULL_TOKEN";
    case TokenType::EOF_TOKEN:
        return "EOF";
    case TokenType::INVALID:
        return "INVALID";
    default:
        return "UNKNOWN";
    }
}
