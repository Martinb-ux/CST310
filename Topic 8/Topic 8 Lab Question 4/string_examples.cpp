#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <vector>

// Example 1: String to float conversion for mathematical calculations
float stringToFloat(const std::string& s) {
    return std::stof(s);
}

// Example 2: Extract numbers from string for coordinates
void extractCoordinates(const std::string& input, float& x, float& y, float& z) {
    size_t start = 0, end;
    std::string coords[3];
    
    for (int i = 0; i < 3; i++) {
        end = input.find(',', start);
        coords[i] = input.substr(start, end - start);
        start = end + 1;
    }
    
    x = stringToFloat(coords[0]);
    y = stringToFloat(coords[1]);
    z = stringToFloat(coords[2]);
}

// Example 3: Create parameter strings for animation
std::string createAnimParam(float time, float amplitude, float frequency) {
    return "t=" + std::to_string(time) + ",a=" + std::to_string(amplitude) + ",f=" + std::to_string(frequency);
}

// Example 4: Parse mathematical expressions
std::string parseFunction(const std::string& expr) {
    std::string result = expr;
    // Replace common mathematical functions
    size_t pos = 0;
    while ((pos = result.find("sin", pos)) != std::string::npos) {
        result.replace(pos, 3, "std::sin");
        pos += 8;
    }
    return result;
}

// Example 5: Format floating point numbers for display
std::string formatFloat(float value, int precision) {
    std::string s = std::to_string(value);
    size_t dot = s.find('.');
    if (dot != std::string::npos && s.length() > dot + precision + 1) {
        s = s.substr(0, dot + precision + 1);
    }
    return s;
}

// Example 6: Split string by delimiter for parsing data
std::vector<std::string> splitString(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Example 7: Remove whitespace for clean parsing
std::string trimWhitespace(const std::string& s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) {
        start++;
    }
    
    auto end = s.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));
    
    return std::string(start, end + 1);
}

// Example 8: Convert string to lowercase for case-insensitive comparison
std::string toLower(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

int main() {
    // Test examples
    std::string coordStr = "1.5,2.3,4.7";
    float x, y, z;
    extractCoordinates(coordStr, x, y, z);
    std::cout << "Coordinates: " << x << ", " << y << ", " << z << std::endl;
    
    std::string animParam = createAnimParam(0.5f, 2.0f, 1.5f);
    std::cout << "Animation params: " << animParam << std::endl;
    
    std::string func = parseFunction("sin(x) + cos(y)");
    std::cout << "Parsed function: " << func << std::endl;
    
    return 0;
}
