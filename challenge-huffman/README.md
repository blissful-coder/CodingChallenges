# Huffman Compression Challenge

A C++ implementation of the Huffman coding algorithm for text compression and decompression. This project demonstrates efficient data compression using binary trees and optimal prefix codes.

## Overview

This implementation provides a complete Huffman coding system that can:
- Compress text files using Huffman encoding
- Decompress previously compressed files
- Display compression statistics and character frequency analysis
- Handle both printable and non-printable characters (including UTF-8)

## Features

- **Bidirectional Operation**: Both compression and decompression modes
- **Automatic Mode Detection**: Detects compressed files by examining file headers
- **Comprehensive Statistics**: Shows compression ratios and space savings
- **UTF-8 Support**: Handles multi-byte characters and binary data
- **Robust Error Handling**: Detailed error messages for various failure modes
- **Memory Efficient**: Uses smart pointers for automatic memory management

## Building the Project

### Prerequisites
- C++ compiler with C++17 support (GCC 7+, Clang 5+, or MSVC 2017+)
- Make utility (optional, for easier building)

### Compilation
```bash
# Simple compilation
g++ -std=c++17 -O2 -o huff huffman.cpp file_utils.cpp

# With debugging symbols
g++ -std=c++17 -g -o huff huffman.cpp file_utils.cpp

# With warnings enabled
g++ -std=c++17 -Wall -Wextra -O2 -o huff huffman.cpp file_utils.cpp
```

## Usage

### Basic Syntax
```bash
./huff <input_file> [output_file]
```

### Compression Examples
```bash
# Compress a text file (output will be input.huf)
./huff document.txt

# Compress with custom output filename
./huff document.txt compressed_doc.huf

# Compress any file type
./huff image.jpg image_compressed.huf
```

### Decompression Examples
```bash
# Decompress (automatically detects .huf files)
./huff document.huf

# Decompress with custom output filename
./huff document.huf restored_document.txt
```

## How It Works

### Compression Process
1. **Frequency Analysis**: Counts occurrence of each character in the input
2. **Tree Construction**: Builds a binary Huffman tree using a min-heap priority queue
3. **Code Generation**: Creates optimal binary codes for each character
4. **Text Encoding**: Replaces characters with their Huffman codes
5. **File Writing**: Stores frequency table header + compressed bit data

### Decompression Process
1. **Header Reading**: Extracts frequency table from compressed file
2. **Tree Reconstruction**: Rebuilds the original Huffman tree
3. **Bit Decoding**: Traverses tree using compressed bits to recover characters
4. **Text Restoration**: Writes decompressed text to output file

### File Format
Compressed files use a custom binary format:
```
[4 bytes: num_characters]
[char + 4-byte frequency] × num_characters
[8 bytes: "HUFFDATA" delimiter]
[8 bytes: total_bits_count]
[variable: packed_compressed_data]
```

## Algorithm Details

### Huffman Tree Construction
- Uses a min-heap priority queue to efficiently build the tree
- Merges nodes with lowest frequencies first
- Handles edge case of single-character files
- Creates optimal prefix-free codes

### Bit Packing
- Compresses bit strings into bytes for storage efficiency
- Preserves exact bit count for accurate decompression
- Handles partial bytes in the final compression step

## Output Examples

### Compression Output
```
Compression mode detected.

--- HUFFMAN CODES ---
'e' -> 00
't' -> 010
'a' -> 011
' ' -> 100
'h' -> 1010
'n' -> 1011
...

--- COMPRESSION STATISTICS ---
Original size: 1024 bytes (8192 bits)
Compressed bits: 4567 bits
Packed size: 571 bytes
Compression ratio: 55.73%
Space saved: 44.27%

Successfully wrote compressed file: document.huf
```

### Decompression Output
```
Decompression mode detected.
Successfully decompressed to: document_decompressed.txt
Decompressed size: 1024 bytes
```

## Performance Characteristics

- **Time Complexity**: O(n log n) for compression, O(n) for decompression
- **Space Complexity**: O(n) where n is the number of unique characters
- **Typical Compression**: 40-60% size reduction on text files
- **Best Case**: Files with highly skewed character distributions
- **Worst Case**: Files with uniform character distributions (may increase size)

## Error Handling

The program handles various error conditions:
- File not found or permission errors
- Corrupted compressed file headers
- Invalid bit sequences during decompression
- Memory allocation failures
- I/O write failures

## Technical Implementation Notes

### Data Structures
- `std::priority_queue` for efficient tree construction
- `std::shared_ptr<Node>` for automatic memory management
- `std::unordered_map` for O(1) code lookups
- `std::map` for ordered frequency analysis

### Character Handling
- Supports all 256 possible byte values
- Displays printable characters as-is
- Shows hex codes for non-printable characters
- Handles common whitespace characters specially

### File I/O
- Binary mode reading/writing for accuracy
- Robust error checking at each I/O operation
- Efficient byte packing for storage optimization

## Testing

Test the implementation with various file types:
```bash
# Text files
./huff sample.txt
./huff sample.huf

# Binary files
./huff image.png
./huff image.huf

# Empty files
touch empty.txt
./huff empty.txt

# Large files
./huff large_document.txt
```

## References

This implementation is based on the Coding Challenges series:
https://codingchallenges.fyi/challenges/challenge-huffman

## License

This project is part of a coding challenge implementation and is provided for educational purposes.
