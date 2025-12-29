# JSON Parse Tree Visualizer

A comprehensive JSON parser with visualization capabilities, built in C++ with a web-based GUI interface.

## Features

### Core JSON Parser (C++)
- Complete lexical analysis with tokenization
- Recursive descent parser that builds abstract syntax trees  
- Support for all JSON data types:
  - Objects `{}`
  - Arrays `[]`
  - Strings `"text"`
  - Numbers (integers and decimals)
  - Booleans (`true`/`false`)
  - Null values
- Proper error handling and validation
- Compliance with JSON specification

### Web-based Visualizer
- **Interactive Parse Tree**: Visual representation of JSON structure
- **Two-panel Interface**: JSON input on left, parse tree on right
- **Real-time Parsing**: Click button to parse and visualize
- **Expandable Tree Nodes**: Click to expand/collapse object and array nodes
- **Color-coded Values**: Different colors for strings, numbers, booleans, etc.
- **Example Templates**: Pre-loaded examples for testing
- **Error Display**: Clear error messages for invalid JSON

## Usage

### Web Visualizer
1. Open `web_visualizer/index.html` in any modern web browser
2. Enter JSON in the left text area
3. Click "Parse JSON" button or press Ctrl+Enter
4. View the parse tree visualization on the right
5. Click on expandable nodes (▼/►) to collapse/expand them
6. Use example buttons to load pre-made JSON samples

### Command Line Parser (C++)
```bash
# Build the parser
make

# Test with JSON files
./jp tests/step1/valid.json
./jp tests/step2/valid.json

# Run all tests
make test
```

## File Structure

```
challenge-json/
├── src/                    # C++ source code
│   ├── main.cpp           # Command-line interface
│   ├── lexer.h/cpp        # Tokenization
│   ├── parser.h/cpp       # JSON validation parser
│   ├── tree_parser.h/cpp  # Parse tree building parser
│   ├── parse_tree.h/cpp   # Tree data structures
│   └── ...
├── web_visualizer/        # Web-based GUI
│   ├── index.html         # Main interface
│   ├── json_parser.js     # JavaScript JSON parser
│   └── visualizer.js      # Tree visualization logic
├── tests/                 # Test JSON files
└── README_VISUALIZER.md   # This file
```

## Technical Details

### C++ Implementation
- **Lexer**: Converts JSON text into tokens (strings, numbers, braces, etc.)
- **Parser**: Validates JSON syntax using recursive descent parsing
- **Tree Parser**: Builds an abstract syntax tree for visualization
- **Parse Tree Nodes**: Represent JSON elements with type information

### JavaScript Implementation
- **Complete rewrite** of the C++ parser logic in JavaScript
- **Token-based parsing** with proper error handling
- **DOM manipulation** for interactive tree visualization
- **Event-driven interface** with real-time updates

### Supported JSON Features
- ✅ Objects with key-value pairs
- ✅ Nested objects and arrays
- ✅ String values with escape sequences
- ✅ Integer and floating-point numbers
- ✅ Boolean values (true/false)
- ✅ Null values
- ✅ Empty objects and arrays
- ✅ Whitespace handling
- ✅ Error detection for invalid syntax

### Example JSON
```json
{
  "company": "Tech Corp",
  "employees": [
    {
      "id": 1,
      "name": "John Doe",
      "skills": ["JavaScript", "Python", "C++"],
      "active": true,
      "salary": null
    }
  ],
  "founded": 2010,
  "public": true
}
```

## Browser Compatibility
- Chrome/Chromium (recommended)
- Firefox
- Safari
- Edge
- Any modern browser with ES6 support

## Development

The project demonstrates:
- **Compiler construction** concepts (lexical analysis, parsing)
- **Data structure design** (tree representation)
- **Web development** (HTML/CSS/JavaScript)
- **Cross-platform compatibility** (C++ backend, web frontend)
- **User interface design** (responsive layout, interactive elements)

## Testing

The visualizer has been tested with:
- Simple objects and arrays
- Deeply nested structures
- Large JSON files
- Invalid JSON (proper error handling)
- Edge cases (empty objects/arrays, special characters)

## Future Enhancements

Potential improvements:
- Syntax highlighting for JSON input
- Export functionality (save parse tree as image)
- JSON validation with detailed error messages
- Support for JSON5 or extended JSON formats
- Performance optimization for very large JSON files
