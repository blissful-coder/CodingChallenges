cmake_minimum_required(VERSION 3.16)
project(JsonParser)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt5
find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

# Set up Qt5
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Include directories
include_directories(src)

# Source files for the core JSON parser
set(CORE_SOURCES
    src/token.cpp
    src/lexer.cpp
    src/parser.cpp
    src/parse_tree.cpp
    src/tree_parser.cpp
    src/file_utils.cpp
)

# Source files for the GUI application
set(GUI_SOURCES
    ${CORE_SOURCES}
    src/json_visualizer.cpp
    src/gui_main.cpp
)

# Create the original command-line parser executable
add_executable(jp src/main.cpp ${CORE_SOURCES})

# Create the GUI visualizer executable
add_executable(json_visualizer ${GUI_SOURCES})

# Link Qt5 libraries to the GUI application
target_link_libraries(json_visualizer Qt5::Core Qt5::Widgets)

# Set output directory
set_target_properties(jp json_visualizer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
