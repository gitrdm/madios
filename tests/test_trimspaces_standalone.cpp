#include "utils/MiscUtils.h"
#include <iostream>
#include <string>

int main() {
    std::string s = "  a  ";
    std::cout << "Input: '" << s << "'\n";
    std::string trimmed = trimSpaces(s);
    std::cout << "Output: '" << trimmed << "'\n";
    return 0;
}
