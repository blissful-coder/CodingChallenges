cmake_minimum_required(VERSION 3.16)
project(JsonParser)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(src)

# Source files for the core JSON parser
set(CORE_SOURCES
    src/token.cpp
    src/lexer.cpp
    src/parser.cpp
    src/file_utils.cpp
)

# Create the original command-line parser executable
add_executable(jp src/main.cpp ${CORE_SOURCES})

# Set output directory
set_target_properties(jp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
