// File: utils/MiscUtils.cpp
// Purpose: Implements miscellaneous utility functions used throughout the ADIOS project.
// Provides string manipulation, tokenization, and file reading utilities.
// All functions are robust to malformed input and documented for developer clarity.
// Part of the ADIOS grammar induction project. See README for usage and structure.

#include "MiscUtils.h"

#include <sstream>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <cctype>

using std::vector;
using std::string;
using std::istream;
using std::stringstream;


// Reads sequences from a file, supporting both ADIOS-style (with '*' and '#') and plain space-separated input.
// Returns a vector of token sequences. Warns once if markers are missing.
vector<vector<string> > readSequencesFromFile(const string &filename) {
    vector<vector<string> > sequences;
    vector<string> tokens;
    string token;
    std::ifstream in(filename.c_str(), std::ios::in);
    if(!in.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
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
                    std::cerr << "Warning: Input line(s) missing '*' or '#' markers. Accepting as plain sequence.\n";
                    warned = true;
                }
            }
            sequences.push_back(tokens);
        }
    }
    in.close();
    return sequences;
}

// Reads all lines from an input stream into a vector of strings.
void getlines(istream &in, vector<string> &lines)
{
    while(!in.eof())
    {
        string line;
        getline(in, line);
        lines.push_back(line);
    }
}

// Tokenizes a string by whitespace.
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

// Tokenizes a string by a custom delimiter.
vector<string> tokenise(const string &line, const char &delimiter)
{
    stringstream ss(line);

    string tok;
    vector<string> tokens;
    while(getline(ss, tok, delimiter))
        if(!tok.empty())
            tokens.push_back(tok);

    return tokens;
}

// Returns an uppercase copy of the input string.
string uppercase(const string &s)
{
    string result = s;
    std::transform(s.begin(), s.end(), result.begin(), [](unsigned char c){ return std::toupper(c); });
    return result;
}

// Returns a lowercase copy of the input string.
string lowercase(const string &s)
{
    string result = s;
    std::transform(s.begin(), s.end(), result.begin(), [](unsigned char c){ return std::tolower(c); });
    return result;
}

// Trims leading and trailing whitespace from a string.
string trimSpaces(const string &s)
{
    if(s.empty()) return s;
    size_t start = s.find_first_not_of(" \t\n\r");
    if(start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\n\r");
    return s.substr(start, end - start + 1);
}
