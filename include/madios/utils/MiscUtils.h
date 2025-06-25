// File: utils/MiscUtils.h
// Purpose: Declares miscellaneous utility functions used throughout the ADIOS project.
// Provides string manipulation, tokenization, and file reading utilities.
// All functions are robust to malformed input and documented for developer clarity.
// Part of the ADIOS grammar induction project. See README for usage and structure.

#ifndef MISC_UTILS_H
#define MISC_UTILS_H

#include <sstream>
#include <istream>
#include <string>
#include <vector>

// Reads all lines from an input stream into a vector of strings.
void getlines(std::istream &in, std::vector<std::string> &lines);

// Tokenizes a string by whitespace.
std::vector<std::string> tokenise(const std::string &line);
// Tokenizes a string by a custom delimiter.
std::vector<std::string> tokenise(const std::string &line, const char &delimiter);

// Returns an uppercase copy of the input string.
std::string uppercase(const std::string &s);
// Returns a lowercase copy of the input string.
std::string lowercase(const std::string &s);
// Trims leading and trailing whitespace from a string.
std::string trimSpaces(const std::string &s);

// Reads sequences from a file, supporting both ADIOS-style (with '*' and '#') and plain space-separated input.
// Returns a vector of token sequences. Warns once if markers are missing.
std::vector<std::vector<std::string> > readSequencesFromFile(const std::string &filename);

// Converts a value to a string using stringstream.
template <typename out_type, typename in_value>
out_type stream_cast(const in_value &t)
{
    std::stringstream ss;
    ss << t;
    out_type result;
    ss >> result;
    return result;
}

// Converts a number to a string.
template <typename T>
std::string num2str(const T &num)
{
    return stream_cast<std::string, T>(num);
}

#endif
