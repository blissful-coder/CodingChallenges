#include "file_utils.h"
#include "lexer.h"
#include "parser.h"
#include <iostream>
#include <stdexcept>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        FileUtils::printUsage();
        return 1;
    }

    try
    {
        std::string fileContent = FileUtils::readFileForParsing(argv[1]);

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
