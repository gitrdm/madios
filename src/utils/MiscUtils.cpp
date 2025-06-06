// File: utils/MiscUtils.cpp
// Purpose: Implements miscellaneous utility functions used throughout the ADIOS project.
// Part of the ADIOS grammar induction project. See README for usage and structure.

#include "MiscUtils.h"

#include <sstream>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <fstream>

using std::vector;
using std::string;
using std::istream;
using std::stringstream;


// Move readSequencesFromFile from main.cpp to MiscUtils.cpp for proper linkage
vector<vector<string> > readSequencesFromFile(const string &filename) {
    vector<vector<string> > sequences;
    vector<string> tokens;
    string token;
    std::ifstream in(filename.c_str(), std::ios::in);
    if(!in.is_open()) {
        std::cout << "Unable to open file: " << filename << std::endl;
        exit(1);
    }
    while(!in.eof()) {
        string line;
        getline(in, line);
        if(line.empty()) continue;
        stringstream ss(line);
        tokens.clear();
        bool has_star = false, has_hash = false;
        while(ss >> token) {
            if(token == "*") has_star = true;
            else if(token == "#") has_hash = true;
            else tokens.push_back(token);
        }
        if(!tokens.empty()) {
            if(!has_star || !has_hash) {
                static bool warned = false;
                if(!warned) {
                    std::cout << "Warning: Input line(s) missing '*' or '#' markers. Accepting as plain sequence.\n";
                    warned = true;
                }
            }
            sequences.push_back(tokens);
        }
    }
    in.close();
    return sequences;
}

void getlines(istream &in, vector<string> &lines)
{
    while(!in.eof())
    {
        string line;
        getline(in, line);
        lines.push_back(line);
    }
}

vector<string> tokenise(const string &line)
{
    stringstream ss(line);

    vector<string> tokens;
    string tok;
    while (ss >> tok)
    {
        tokens.push_back(tok);
    }

    return tokens;
}

vector<string> tokenise(const string &line, const char &delimiter)
{
    stringstream ss(line);

    string tok;
    vector<string> tokens;
    while(getline(ss, tok, delimiter))
        if(tok.size() > 0)
            tokens.push_back(tok);

    return tokens;
}

string uppercase(const string &s)
{
    string result = s;
    transform(s.begin(), s.end(), result.begin(), toupper);
    return result;
}

string lowercase(const string &s)
{
    string result = s;
    transform(s.begin(), s.end(), result.begin(), tolower);
    return result;
}

string trimSpaces(const string &s)
{
    printf("trimSpaces input ptr: %p, size: %zu\n", (const void*)s.c_str(), s.size());
    fflush(stdout);
    string str = s;
    size_t t = str.find('\t');
    while (t != string::npos) {
        str[t] = ' ';
        t = str.find('\t', t + 1);
    }
    t = str.find('\n');
    while (t != string::npos) {
        str[t] = ' ';
        t = str.find('\n', t + 1);
    }
    if (str.empty()) return "";
    size_t n = str.find_first_not_of(" ");
    size_t k = str.find_last_not_of(" ");
    if (n == string::npos) return "";
    if (k == string::npos || k < n) return "";
    return str.substr(n, k - n + 1);
}
