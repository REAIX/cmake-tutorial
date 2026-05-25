#include "string_utils.h"
#include <algorithm>
#include <cctype>
#include <sstream>

namespace string_utils {

std::string reverse(const std::string& str) {
    std::string result = str;
    std::reverse(result.begin(), result.end());
    return result;
}

std::string to_upper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
    return result;
}

int count_words(const std::string& str) {
    std::istringstream iss(str);
    int count = 0;
    std::string word;
    while (iss >> word) {
        ++count;
    }
    return count;
}

}
