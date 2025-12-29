# JSON Parser

A modular JSON parser implementation in C++ that validates JSON syntax according to the JSON specification.

## Overview

This JSON parser has been refactored from a single monolithic file into a clean, modular architecture with separate classes and files for better maintainability and extensibility.

## Project Structure

```
src/
├── token.h          # Token types and structures
├── token.cpp        # Token utility functions
├── lexer.h          # Lexer class declaration
├── lexer.cpp        # Lexer implementation (tokenization)
├── parser.h         # Parser class declaration
├── parser.cpp       # Parser implementation (syntax validation)
├── file_utils.h     # File operations namespace
├── file_utils.cpp   # File reading utilities
└── main.cpp         # Main application entry point
```

## Architecture

### Components

1. **Token System** (`token.h/cpp`)
   - Defines all JSON token types (LBRACE, STRING, NUMBER, etc.)
   - Token structure for position tracking
   - Utility functions for token name conversion

2. **Lexer Class** (`lexer.h/cpp`)
   - Tokenizes JSON input character by character
   - Handles strings, numbers, keywords, and punctuation
   - Provides detailed lexical analysis with position tracking

3. **Parser Class** (`parser.h/cpp`)
   - Recursive descent parser for JSON validation
   - Validates JSON syntax according to specification
   - Handles nested objects, arrays, and all JSON value types

4. **File Utilities** (`file_utils.h/cpp`)
   - File reading operations in `FileUtils` namespace
   - Robust error handling for file operations
   - Usage message functionality

5. **Main Application** (`main.cpp`)
   - Command-line interface
   - Ties all components together
   - Exception handling and program flow

## Building

### Prerequisites
- C++11 compatible compiler (g++, clang++, etc.)
- Make

### Build Commands
```bash
# Build the project
make

# Clean build artifacts
make clean

# Build and run tests
make test

# Run tests with verbose output (shows lexer traces)
make test-verbose
```

## Usage

```bash
# Basic usage
./jp filename.json

# Example
./jp tests/step1/valid.json
```

### Command Line Interface
- **Input**: Path to a JSON file
- **Output**: "Valid JSON" or "Invalid JSON"
- **Exit Code**: 0 for valid JSON, 1 for invalid JSON or errors

## Testing

The parser includes comprehensive tests across 4 steps:

### Test Categories
- **Step 1**: Basic empty objects
- **Step 2**: String key-value pairs
- **Step 3**: All JSON value types (true, false, null, string, number)
- **Step 4**: Complex nested structures (objects, arrays)

### Running Tests
```bash
# Run all tests with clean output
make test

# Run tests with detailed lexer output
make test-verbose
```

## JSON Features Supported

- ✅ Objects with string keys
- ✅ String values (with escape sequences)
- ✅ Number values (integers)
- ✅ Boolean values (true, false)
- ✅ Null values
- ✅ Nested objects
- ✅ Arrays (empty and with values)
- ✅ Proper comma handling
- ✅ Whitespace handling

## Error Handling

The parser provides robust error handling:
- File reading errors (file not found, permissions, etc.)
- Invalid JSON syntax detection
- Position tracking for debugging
- Clear error messages

## Example Valid JSON

```json
{
  "name": "John",
  "age": 30,
  "active": true,
  "address": {},
  "hobbies": []
}
```

## Example Invalid JSON

```json
{
  "name": "John",
  "age": 30,
}  // Trailing comma - invalid
```

## Development

### Code Style
- C++11 standard
- Clear class separation
- Comprehensive error handling
- Position tracking for debugging

### Extending the Parser
The modular architecture makes it easy to:
- Add new token types
- Extend parsing capabilities
- Modify lexical analysis
- Add new output formats

## License

This project is part of a coding challenge implementation.
