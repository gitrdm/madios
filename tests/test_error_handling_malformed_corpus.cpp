// Test: Error handling for malformed corpus input
// This test will attempt to load a malformed corpus and verify that the program handles the error gracefully.
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <fstream>
#include <cstdlib>

TEST_CASE("Malformed corpus input triggers error handling", "[error][corpus]") {
    // Write a malformed corpus to a temp file
    const char* filename = "malformed_corpus.txt";
    std::ofstream ofs(filename);
    ofs << "This is not a valid corpus format!\n";
    ofs.close();

    // Attempt to run the CLI or core function that loads the corpus
    int result = std::system("./madios --input malformed_corpus.txt > /dev/null 2>&1");
    // Expect non-zero exit code for error
    REQUIRE(result != 0);
    std::remove(filename);
}

TEST_CASE("Empty corpus input triggers error handling", "[error][corpus][empty]") {
    // Write an empty corpus to a temp file
    const char* filename = "empty_corpus.txt";
    std::ofstream ofs(filename);
    ofs.close();

    int result = std::system("./madios --input empty_corpus.txt > /dev/null 2>&1");
    REQUIRE(result != 0);
    std::remove(filename);
}
