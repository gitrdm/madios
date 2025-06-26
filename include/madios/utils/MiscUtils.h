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

/**
 * @brief Reads all lines from an input stream into a vector of strings.
 * @param in Input stream to read from.
 * @param lines Vector to store the lines.
 */
void getlines(std::istream &in, std::vector<std::string> &lines);

/**
 * @brief Tokenizes a string by whitespace.
 * @param line The string to tokenize.
 * @return Vector of tokens split by whitespace.
 */
std::vector<std::string> tokenise(const std::string &line);
/**
 * @brief Tokenizes a string by a custom delimiter.
 * @param line The string to tokenize.
 * @param delimiter The delimiter character.
 * @return Vector of tokens split by the delimiter.
 */
std::vector<std::string> tokenise(const std::string &line, const char &delimiter);

/**
 * @brief Returns an uppercase copy of the input string.
 * @param s The input string.
 * @return Uppercase version of the string.
 */
std::string uppercase(const std::string &s);
/**
 * @brief Returns a lowercase copy of the input string.
 * @param s The input string.
 * @return Lowercase version of the string.
 */
std::string lowercase(const std::string &s);
/**
 * @brief Trims leading and trailing whitespace from a string.
 * @param s The input string.
 * @return Trimmed string.
 */
std::string trimSpaces(const std::string &s);

/**
 * @brief Reads sequences from a file, supporting both ADIOS-style (with '*' and '#') and plain space-separated input.
 * @param filename The file to read from.
 * @return Vector of token sequences. Warns once if markers are missing.
 */
std::vector<std::vector<std::string> > readSequencesFromFile(const std::string &filename);

/**
 * @brief Converts a value to a string using stringstream.
 * @tparam out_type The output type.
 * @tparam in_value The input value type.
 * @param t The value to convert.
 * @return The converted value as out_type.
 */
template <typename out_type, typename in_value>
out_type stream_cast(const in_value &t)
{
    std::stringstream ss;
    ss << t;
    out_type result;
    ss >> result;
    return result;
}

/**
 * @brief Converts a number to a string.
 * @tparam T The numeric type.
 * @param num The number to convert.
 * @return The number as a string.
 */
template <typename T>
std::string num2str(const T &num)
{
    return stream_cast<std::string, T>(num);
}

#endif
