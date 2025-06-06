// File: utils/MiscUtils.cpp
// Purpose: Implements miscellaneous utility functions used throughout the ADIOS project.
// Part of the ADIOS grammar induction project. See README for usage and structure.

#include "MiscUtils.h"

#include <sstream>
#include <algorithm>
#include <iostream>
#include <cstdio>

using std::vector;
using std::string;
using std::istream;
using std::stringstream;


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
    unsigned int t;
    while ((t = str.find('\t')) != string::npos) str[t] = ' ';
    while ((t = str.find('\n')) != string::npos) str[t] = ' ';
    if (str.empty()) return "";
    unsigned int n = str.find_first_not_of(" ");
    unsigned int k = str.find_last_not_of(" ");
    if (n == string::npos) return "";
    if (k == string::npos || k < n) return "";
    return str.substr(n, k - n + 1);
}
