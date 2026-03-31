# Topic 8 Lab Question 4: C++ String Manipulation Examples for Project 7

## Overview
This lab question provides several examples of C++ code for manipulating strings, which would be useful in Project 7. These examples demonstrate practical string handling techniques for parsing configuration files, processing mathematical expressions, formatting display text, and handling user input.

## Files
- `string_examples.cpp` - C++ string manipulation examples
- `README.md` - This documentation

## Compilation

### macOS/Linux
```bash
g++ string_examples.cpp -o string_examples
```

### Windows (MinGW)
```bash
g++ string_examples.cpp -o string_examples.exe
```

### Alternative with C++11 standard (recommended)
```bash
g++ -std=c++11 string_examples.cpp -o string_examples
```

## Running

### macOS/Linux
```bash
./string_examples
```

### Windows
```bash
string_examples.exe
```

## String Manipulation Examples

The `string_examples.cpp` file provides useful string manipulation functions for Project 7:

### 1. **String to Float Conversion** - Parse numeric values from text
```cpp
float stringToFloat(const std::string& s) {
    return std::stof(s);
}
```
**Use case**: Converting configuration values or user input to numbers for calculations.

### 2. **Coordinate Extraction** - Parse "x,y,z" format strings
```cpp
void extractCoordinates(const std::string& input, float& x, float& y, float& z)
```
**Use case**: Loading 3D positions from configuration files or parsing user input.

### 3. **Animation Parameters** - Create parameter strings for animations
```cpp
std::string createAnimParam(float time, float amplitude, float frequency)
```
**Use case**: Generating dynamic parameter strings for animation systems.

### 4. **Mathematical Expression Parsing** - Convert math notation to C++ syntax
```cpp
std::string parseFunction(const std::string& expr)
```
**Use case**: Converting user-friendly math expressions like "sin(x)" to C++ syntax.

### 5. **Float Formatting** - Format numbers for display
```cpp
std::string formatFloat(float value, int precision)
```
**Use case**: Displaying scores, coordinates, or other numerical values with controlled precision.

### 6. **String Splitting** - Parse delimited data
```cpp
std::vector<std::string> splitString(const std::string& s, char delimiter)
```
**Use case**: Parsing CSV data, configuration files, or command-line arguments.

### 7. **Whitespace Trimming** - Clean input strings
```cpp
std::string trimWhitespace(const std::string& s)
```
**Use case**: Cleaning user input or configuration file entries.

### 8. **Case Conversion** - Case-insensitive string comparison
```cpp
std::string toLower(const std::string& s)
```
**Use case**: Making string comparisons case-insensitive for commands or configuration keys.

## Applications in Project 7

These string manipulation examples are particularly useful for:

- **Configuration Files**: Parse settings, colors, positions, and animation parameters
- **User Input Processing**: Handle commands, coordinate input, and mathematical expressions
- **Display Formatting**: Format scores, coordinates, and status messages
- **Data Loading**: Parse level data, object positions, and animation sequences
- **Debug Output**: Format debug information and error messages

## Technical Specifications

- **Language**: C++ standard library (C++11 compatible)
- **Dependencies**: `<string>`, `<algorithm>`, `<cctype>`, `<sstream>`, `<vector>`, `<iostream>`
- **Platform**: Cross-platform compatible (macOS, Linux, Windows)
- **Compiler**: g++ (Linux/macOS), MinGW (Windows), or any C++11 compliant compiler
- **Compilation**: Simple g++ compilation, no external libraries needed
- **Standards**: Uses only standard C++ library functions for maximum compatibility
