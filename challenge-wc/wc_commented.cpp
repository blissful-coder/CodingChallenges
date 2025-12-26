#include <iostream>              // Standard input/output stream operations
                                     // BETTER C++: Prefer specific includes over broad ones when possible

#include <fstream>               // File stream operations for reading/writing files
                                     // BETTER C++: This is appropriate, std::ifstream/ofstream are modern

using namespace std;             // Import all std namespace identifiers into global scope
                                     // BETTER C++: AVOID! Use explicit std:: prefixes or specific using declarations
                                     // Should be: using std::cout; using std::cin; etc. or just use std::

// Count the number of lines in the input file
// BETTER C++: Function naming should be concise: countLines() instead of countNumberOfLines()
// BETTER C++: Pass by const reference for efficiency: const std::string& filename or use a class
int countNumberOfLines(ifstream &file)  // Takes file stream by reference (good for avoiding copies)
{                                    // BETTER C++: Use consistent brace style (prefer same-line opening brace)
    int numOfLines = 0;              // Counter variable initialized to zero
                                     // BETTER C++: Use size_t instead of int for counting (avoids signed/unsigned issues)
                                     // BETTER C++: Use more concise naming: lineCount or lines
    string line;                     // String to store each line read from file
                                     // BETTER C++: This is fine, but could use std::string explicitly

    while (getline(file, line))      // Read file line by line until EOF
        numOfLines++;                // Increment counter for each line found
                                     // BETTER C++: This is efficient and modern, good approach
                                     // BETTER C++: Could use range-based counting with std algorithms

    return numOfLines;               // Return the total count
}                                    // BETTER C++: Overall, this function logic is reasonable

// Count the number of bytes in the input file
// BETTER C++: Again, prefer shorter name: countBytes()
int countNumberOfBytes(ifstream &file)   // File stream parameter by reference
{
    int numOfBytes = 0;              // Byte counter initialized to zero
                                     // BETTER C++: Use size_t for file sizes (int may overflow on large files)

    file.seekg(0, ios::end);         // Move file pointer to end of file
                                     // BETTER C++: This is appropriate for getting file size
    numOfBytes = file.tellg();       // Get position (which equals file size in bytes)
                                     // BETTER C++: Should check for errors: if (file.tellg() != -1)

    return numOfBytes;               // Return total byte count
}                                    // BETTER C++: This approach is fine, but could use std::filesystem::file_size()

// Count the number of words in the input file
// BETTER C++: Shorter function name preferred
int countNumberOfWords(ifstream &file)   // File stream by reference
{
    int numOfWords = 0;              // Word counter
                                     // BETTER C++: Use size_t instead of int
    string line;                     // String for storing each line

    while (getline(file, line))      // Process file line by line
    {                                // Opening brace on new line (inconsistent style)
                                     // BETTER C++: Be consistent with brace placement
        int index = 0;               // Index for character-by-character processing
                                     // BETTER C++: Use size_t for array/string indices
                                     // BETTER C++: This entire approach is inefficient
        while (index < line.length()) // Loop through each character in line
        {                            // BETTER C++: Use range-based for loop or algorithms
            if (isspace(line[index])) // Check if current character is whitespace
            {                        // BETTER C++: Use std::isspace explicitly
                index++;             // Skip whitespace characters
                continue;            // Continue to next iteration
                ;                    // Unnecessary semicolon after continue
                                     // BETTER C++: Remove the extra semicolon
            }

            numOfWords++;            // Found start of a word, increment counter
                                     // BETTER C++: This logic is correct but verbose

            while (index < line.length() && !isspace(line[index])) // Skip non-whitespace chars
                index++;             // Move to next character
                                     // BETTER C++: Use std::find_if or similar STL algorithm
        }
    }

    return numOfWords;               // Return word count
}                                    // BETTER C++: Replace entire function with std::istringstream word extraction

// Count the number of characters in the input file
// BETTER C++: Function name could be shorter
int countNumberOfCharacters(ifstream &file) // File stream parameter
{
    int numOfCharacters = 0;         // Character counter
                                     // BETTER C++: Use size_t for large counts
    char ch;                         // Single character variable for reading

    while (file.get(ch))             // Read file character by character
        numOfCharacters++;           // Increment counter for each character
                                     // BETTER C++: This is inefficient, could read entire file at once

    return numOfCharacters;          // Return character count
}                                    // BETTER C++: Use file size or string.length() after reading entire file

// Validate the option - i.e. -l, -w, -c, -m
// BETTER C++: Function name is descriptive but could be shorter: isValidOption()
bool isOptionValid(string option)   // Takes option string by value (creates unnecessary copy)
{                                    // BETTER C++: Pass by const reference: const std::string& option
    if (option.length() != 2 || (option.length() == 2 && option[0] != '-'))
                                     // Check if option is exactly 2 chars and starts with '-'
                                     // BETTER C++: Redundant check (option.length() == 2 checked twice)
                                     // BETTER C++: Could use option.size() instead of length()
        return false;                // Return false for invalid option
    return true;                     // Return true for valid format
}                                    // BETTER C++: Use std::set or switch for valid option checking

int main(int argc, char *argv[])     // Standard main function with command line arguments
{                                    // argc = argument count, argv = argument vector
    if (argc < 2 || argc > 3)        // Check if argument count is valid (program name + 1-2 args)
    {                                // BETTER C++: Magic numbers should be named constants
        cerr << "Usage: " << argv[0] << " <-l | -w | -c | -m> <filename>" << endl;
                                     // Print usage message to error stream
                                     // BETTER C++: This is appropriate use of cerr
        return 1;                    // Return non-zero to indicate error
    }                                // BETTER C++: Consider using enum for return codes

    // Handle read from standard input if no filename is specified
    // i.e. cat file.txt | ccwc -l
    if (argc == 2 && cin.peek())     // If 2 args and stdin has data available
    {                                // BETTER C++: cin.peek() approach is fragile
        if (!isOptionValid(string(argv[1]))) // Validate the option argument
        {                            // BETTER C++: Unnecessary string conversion, just pass argv[1]
            cerr << "Usage: " << argv[0] << " <-l | -w | -c | -m> <filename>" << endl;
                                     // Error message for invalid option
            return 1;                // Exit with error code
        }

        char option = string(argv[1])[1]; // Extract the option character (after -)
                                     // BETTER C++: Direct access: option = argv[1][1]
                                     // BETTER C++: Should validate string length before indexing

        string fileName = "inputFile.txt"; // Hard-coded temporary filename
                                     // BETTER C++: AVOID hard-coded filenames, use std::tmpfile()
                                     // BETTER C++: Better yet, avoid temporary files entirely

        ofstream outputFile(fileName); // Create output file stream
                                     // BETTER C++: This approach is unnecessarily complex

        if (!outputFile)             // Check if file creation failed
        {                            // BETTER C++: Good error checking
            cerr << "Error creating the file " << fileName << endl;
            return 1;
        }

        string inputText;            // String to store input lines

        // Read from standard input (stdin) and write into the inputFile.txt
        while (getline(cin, inputText)) // Read line by line from stdin
            outputFile << inputText << endl; // Write each line to temporary file
                                     // BETTER C++: Avoid temporary file, store in std::stringstream or std::string

        outputFile.close();          // Close the output file
                                     // BETTER C++: RAII would handle this automatically

        ifstream inputFile(fileName); // Open the temporary file for reading
                                     // BETTER C++: Completely unnecessary step

        if (!inputFile.is_open())    // Check if file opening failed
        {                            // BETTER C++: Good error checking practice
            cerr << "Error: Could not open file " << fileName << endl;
            return 1;
        }

        switch (option)              // Switch on the option character
        {                            // BETTER C++: This switch logic is repeated multiple times
        case 'l':                    // Line count option
            cout << countNumberOfLines(inputFile) << endl;
            break;                   // BETTER C++: Function calls are appropriate
        case 'c':                    // Byte count option
            cout << countNumberOfBytes(inputFile) << endl;
            break;
        case 'm':                    // Character count option
            cout << countNumberOfCharacters(inputFile) << endl;
            break;
        case 'w':                    // Word count option
            cout << countNumberOfWords(inputFile) << endl;
            break;
        default:                     // Handle invalid option
            cerr << "Error: Invalid option. Use -l, -w, -c or -m." << endl;
            break;                   // BETTER C++: Should return error code instead of break
        }

        inputFile.close();           // Close input file
                                     // BETTER C++: RAII would handle automatically

        if (remove(fileName.c_str()) == 0) // Delete temporary file
        {                            // BETTER C++: remove() is C-style, use std::filesystem::remove()
            // File successfully deleted  // Empty success block
        }
        else
        {
            cerr << "Error: Deleting the file " << fileName << endl;
            return 1;                // BETTER C++: This entire temp file approach should be avoided
        }

        return 0;                    // Success return
    }
    // Handle default option if no options are provided
    // i.e. ccwc file.txt
    else if (argc == 2)              // If only filename provided (no option)
    {
        ifstream inputFile(argv[1]); // Open the specified file
                                     // BETTER C++: Good approach, direct file opening

        if (!inputFile.is_open())    // Check file opening success
        {
            cerr << "Error: Could not open file " << argv[1] << endl;
            return 1;                // BETTER C++: Good error handling
        }

        cout << countNumberOfLines(inputFile) << " "; // Print line count
                                     // BETTER C++: Multiple function calls require multiple file reads

        inputFile.clear();           // Clear any error flags from previous operation
        inputFile.seekg(0, ios::beg); // Reset file position to beginning
                                     // BETTER C++: This file reset pattern is repeated - could be a helper function

        cout << countNumberOfWords(inputFile) << " "; // Print word count

        inputFile.clear();           // Clear error flags again
        inputFile.seekg(0, ios::beg); // Reset file position again
                                     // BETTER C++: Inefficient - reading file multiple times

        cout << countNumberOfBytes(inputFile) << " " << argv[1] << endl; // Print byte count and filename

        inputFile.close();           // Close file
                                     // BETTER C++: Should read file once and calculate all counts

        return 0;
    }

    // Handle regular option if all options are provided
    // i.e. ccwc -l file.txt
    if (argc == 3)                   // If both option and filename provided
    {
        if (!isOptionValid(string(argv[1]))) // Validate option format
        {                            // BETTER C++: Repeated validation logic
            cerr << "Usage: " << argv[0] << " <-l | -w | -c | -m> <filename>" << endl;
            return 1;
        }

        char option = string(argv[1])[1]; // Extract option character
                                     // BETTER C++: Repeated logic, should be extracted to function

        ifstream inputFile(argv[2]); // Open specified file

        if (!inputFile.is_open())    // Check file opening
        {
            cerr << "Error: Could not open file " << argv[2] << endl;
            return 1;
        }

        switch (option)              // Same switch logic as before
        {                            // BETTER C++: Code duplication - extract to function
        case 'l':
            cout << countNumberOfLines(inputFile) << " " << argv[2] << endl;
            break;
        case 'c':
            cout << countNumberOfBytes(inputFile) << " " << argv[2] << endl;
            break;
        case 'm':
            cout << countNumberOfCharacters(inputFile) << " " << argv[2] << endl;
            break;
        case 'w':
            cout << countNumberOfWords(inputFile) << " " << argv[2] << endl;
            break;
        default:
            cerr << "Error: Invalid option. Use -l, -w, -c or -m." << endl;
            return 1;                // BETTER C++: Consistent error handling
        }

        inputFile.close();           // Close file

        return 0;
    }

    return 0;                        // Default return (should never reach here)
}                                    // BETTER C++: Overall structure needs refactoring into classes

/* 
OVERALL BETTER C++ RECOMMENDATIONS:

1. ELIMINATE: using namespace std;
2. USE CLASSES: WordCounter and CommandLineParser classes
3. USE RAII: Automatic resource management
4. AVOID TEMPORARY FILES: Use string streams for stdin
5. READ ONCE: Calculate all counts in single file pass
6. USE STL ALGORITHMS: Replace manual loops with std::count, std::distance, etc.
7. CONSISTENT NAMING: Use camelCase or snake_case consistently
8. ERROR HANDLING: Use exceptions or std::optional for error cases
9. CONST CORRECTNESS: Mark functions and parameters const where possible
10. SMART POINTERS: Use std::unique_ptr for dynamic allocation if needed
11. MODERN TYPES: Use size_t for counting, std::string_view for string parameters
12. ELIMINATE CODE DUPLICATION: Extract common logic into functions
*/
