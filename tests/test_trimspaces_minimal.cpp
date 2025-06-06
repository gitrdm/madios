#include <string>
#include <iostream>

// std::string trimSpaces(const std::string &s) {
//     std::string str = s;
//     unsigned int t;
//     while ((t = str.find('\t')) != std::string::npos) str[t] = ' ';
//     while ((t = str.find('\n')) != std::string::npos) str[t] = ' ';
//     if (str.empty()) return "";
//     unsigned int n = str.find_first_not_of(" ");
//     unsigned int k = str.find_last_not_of(" ");
//     if (n == std::string::npos) return "";
//     if (k == std::string::npos || k < n) return "";
//     return str.substr(n, k - n + 1);
// }

// int main() {
//     std::string s = "  a  ";
//     std::cout << "Input: '" << s << "'\n";
//     std::string trimmed = trimSpaces(s);
//     std::cout << "Output: '" << trimmed << "'\n";
//     return 0;
// }

int main() {
    std::string s = "hello";
    std::cout << s << std::endl;
    return 0;
}