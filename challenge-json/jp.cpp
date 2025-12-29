#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <cctype>

// ---------------- File read (your code) ----------------

std::string readFileForParsing(const std::string &filename)
{
    std::ifstream rfile(filename, std::ios::binary);
    if (!rfile.is_open())
        throw std::runtime_error("Could not open file: " + filename);

    rfile.seekg(0, std::ios::end);
    std::streampos pos = rfile.tellg();
    if (pos < 0)
        throw std::runtime_error("Failed to determine file size: " + filename);

    std::size_t size = static_cast<std::size_t>(pos);
    rfile.seekg(0, std::ios::beg);
    if (size == 0)
        return {};

    std::string content(size, '\0');
    rfile.read(&content[0], size);
    if (rfile.fail() || static_cast<std::size_t>(rfile.gcount()) != size)
        throw std::runtime_error("Failed to read complete file: " + filename);

    return content;
}

void printUsage()
{
    std::cout << "\nUsage: ./jp <file_name>\nExample: ./jp test.json\n";
}

// ---------------- Lexer ----------------

enum class TokenType
{
    LBRACE,
    RBRACE,
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

class Lexer
{
    const std::string &s;
    std::size_t i = 0;

    void skipWs()
    {
        while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i])))
            i++;
    }

public:
    explicit Lexer(const std::string &input) : s(input) {}

    Token next()
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

    Token parseString()
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

    Token parseKeyword()
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

    Token parseNumber()
    {
        std::size_t start = i;

        while (i < s.size() && std::isdigit(s[i]))
        {
            i++;
        }

        std::cout << "[LEX] " << tokenName(TokenType::NUMBER) << " at pos " << start << "\n";
        return {TokenType::NUMBER, start, '\0'};
    }

    const char *tokenName(TokenType t)
    {
        switch (t)
        {
        case TokenType::LBRACE:
            return "LBRACE";
        case TokenType::RBRACE:
            return "RBRACE";
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
};

// ---------------- Parser ----------------

class Parser
{
    Lexer lex;
    Token cur;

    void advance() { cur = lex.next(); }

public:
    explicit Parser(Lexer l) : lex(std::move(l)) { advance(); }

    bool parse()
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

    bool parseKeyValue()
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
    
    bool parseValue()
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

        return false;
    }
};

// ---------------- main ----------------

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printUsage();
        return 1;
    }

    try
    {
        std::string fileContent = readFileForParsing(argv[1]);

        Lexer lexer(fileContent);
        Parser parser(std::move(lexer));

        bool ok = parser.parse();
        std::cout << (ok ? "Valid JSON\n" : "Invalid JSON\n");
        return ok ? 0 : 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
