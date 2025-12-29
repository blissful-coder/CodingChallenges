#include "lexer.h"
#include <iostream>
#include <cctype>

Lexer::Lexer(const std::string &input) : s(input) {}

void Lexer::skipWs()
{
    while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i])))
        i++;
}

Token Lexer::next()
{
    skipWs();
    if (i >= s.size())
        return {TokenType::EOF_TOKEN, i, '\0'};

    char c = s[i];
    if (c == '{')
    {
        i++;
        std::cout << "[LEX] " << tokenName(TokenType::LBRACE) << " at pos " << (i - 1) << "\n";
        return {TokenType::LBRACE, i - 1, '\0'};
    }
    if (c == '[')
    {
        i++;
        std::cout << "[LEX] " << tokenName(TokenType::LBRACKET) << " at pos " << (i - 1) << "\n";
        return {TokenType::LBRACKET, i - 1, '\0'};
    }
    if (c == ']')
    {
        i++;
        std::cout << "[LEX] " << tokenName(TokenType::RBRACKET) << " at pos " << (i - 1) << "\n";
        return {TokenType::RBRACKET, i - 1, '\0'};
    }
    if (c == '}')
    {
        i++;
        std::cout << "[LEX] " << tokenName(TokenType::RBRACE) << " at pos " << (i - 1) << "\n";
        return {TokenType::RBRACE, i - 1, '\0'};
    }
    if (c == ':')
    {
        i++;
        std::cout << "[LEX] " << tokenName(TokenType::COLON) << " at pos " << (i - 1) << "\n";
        return {TokenType::COLON, i - 1, '\0'};
    }
    if (c == ',')
    {
        i++;
        std::cout << "[LEX] " << tokenName(TokenType::COMMA) << " at pos " << (i - 1) << "\n";
        return {TokenType::COMMA, i - 1, '\0'};
    }
    if (c == '"')
        return parseString();

    if (std::isalpha(c))
        return parseKeyword();
    if (std::isdigit(c))
        return parseNumber();

    i++;
    return {TokenType::INVALID, i - 1, c};
}

Token Lexer::parseString()
{
    std::size_t start = i;
    i++; // skip opening quote

    while (i < s.size() && s[i] != '"')
    {
        if (s[i] == '\\')
        {
            i++; // skip escape character
            if (i >= s.size())
                return {TokenType::INVALID, start, '"'};
            i++; // skip escaped character
        }
        else
        {
            i++;
        }
    }

    if (i >= s.size())
        return {TokenType::INVALID, start, '"'};

    i++; // skip closing quote
    std::cout << "[LEX] " << tokenName(TokenType::STRING) << " at pos " << start << "\n";
    return {TokenType::STRING, start, '\0'};
}

Token Lexer::parseKeyword()
{
    std::size_t start = i;
    char start_char = s[i];
    std::string keyword;

    while (i < s.size() && ((s[i]>='a' && s[i]<='z') || (s[i]>='A' && s[i]<='Z')))
    {
        i++;
    }

    keyword = s.substr(start,i-start);
    TokenType result = TokenType::INVALID;
    if (keyword == "true")
        result = TokenType::TRUE;
    else if (keyword == "false")
        result = TokenType::FALSE;
    else if (keyword == "null")
        result = TokenType::NULL_TOKEN;

    if (result == TokenType::INVALID)
        return {TokenType::INVALID, start, start_char};

    std::cout << "[LEX] " << tokenName(result) << " at pos " << start << "\n";
    return {result, start, '\0'};
}

Token Lexer::parseNumber()
{
    std::size_t start = i;

    while (i < s.size() && std::isdigit(s[i]))
    {
        i++;
    }

    std::cout << "[LEX] " << tokenName(TokenType::NUMBER) << " at pos " << start << "\n";
    return {TokenType::NUMBER, start, '\0'};
}
