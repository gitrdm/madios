// File: main.cpp
// Purpose: Entry point for the madios project. Handles input/output and program flow for grammar induction using the ADIOS algorithm.
// Part of the ADIOS grammar induction project. See README for usage and structure.
//
// Usage: ./madios test/corpus.txt
//
// This file is a good starting point for understanding how the program operates.

#include "MiscUtils.h"
#include "RDSGraph.h"
#include "special.h"
#include "TimeFuncs.h"
#include "utils/json.hpp"

#include <sstream>
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <fstream>
#include <iomanip>

using std::vector;
using std::pair;
using std::string;
using std::stringstream;
using std::ifstream;
using std::ios;
using std::cout;
using std::endl;

vector<vector<string> > readSequencesFromFile(const string &filename)
{
    vector<vector<string> > sequences;
    vector<string> tokens;
    string token;

    ifstream in(filename.c_str(), ios::in);
    if(!in.is_open())
    {
        cout << "Unable to open file: " << filename << endl;
        exit(1);
    }

    while(!in.eof())
    {
        string line;
        getline(in, line);

        if(line.size() > 0)
        {
            stringstream ss(line);
            while(!ss.eof())
            {
                ss >> token;

                if(token == "*")
                    tokens.clear();
                else if(token == "#")
                {
                    sequences.push_back(tokens);
                    break;
                }
                else
                    tokens.push_back(token);
            }
        }
    }
    in.close();

    return sequences;
}

int main(int argc, char *argv[])
{
    bool json_mode = false;
    int positional_argc = 0;
    for(int i = 1; i < argc; ++i) {
        if(std::string(argv[i]) == "--json") {
            json_mode = true;
            break;
        } else {
            positional_argc++;
        }
    }
    if(positional_argc < 5) {
        cout << "Usage:" << endl;
        cout << "madios <filename> <eta> <alpha> <context_size> <coverage> [--json] ---OPTIONAL--- <number_of_new_sequences>" << endl;
        exit(1);
    }
    vector<vector<string> > sequences = readSequencesFromFile(argv[1]);
    RDSGraph testGraph(sequences);
    testGraph.setQuiet(json_mode); // Suppress verbose output if --json is set
    double startTime = getTime();
    testGraph.distill(ADIOSParams(atof(argv[2]), atof(argv[3]), atoi(argv[4]), atof(argv[5])));
    double endTime = getTime();
    if(json_mode) {
        nlohmann::json j;
        j["corpus"] = sequences;
        // Search paths
        std::vector<std::vector<std::string>> search_paths;
        for(const auto& path : testGraph.getPaths()) {
            std::vector<std::string> s;
            for(unsigned int idx : path) {
                s.push_back(testGraph.getNodeName(idx));
            }
            search_paths.push_back(s);
        }
        j["search_paths"] = search_paths;
        // Lexicon
        std::vector<nlohmann::json> lexicon;
        const auto& nodes = testGraph.getNodes();
        for(size_t i = 0; i < nodes.size(); ++i) {
            nlohmann::json node_j;
            node_j["id"] = i;
            node_j["type"] = nodes[i].type;
            node_j["string"] = testGraph.getNodeString(i);
            std::vector<unsigned int> parents;
            for(const auto& p : nodes[i].parents) parents.push_back(p.first);
            node_j["parents"] = parents;
            lexicon.push_back(node_j);
        }
        j["lexicon"] = lexicon;
        // Grammar (PCFG)
        std::stringstream grammar_ss;
        testGraph.convert2PCFG(grammar_ss);
        j["grammar"] = grammar_ss.str();
        j["timing"] = endTime - startTime;
        std::cout << std::setw(2) << j << std::endl;
        return 0;
    } else {
        cout << "eta = " << argv[2] << endl;
        cout << "alpha = " << argv[3] << endl;
        cout << "contextSize = " << argv[4] << endl;
        cout << "overlapThreshold = " << argv[5] << endl;
        cout << "BEGIN CORPUS ----------" << endl;
        for(unsigned int i = 0; i < sequences.size(); i++) {
            for(unsigned int j = 0; j < sequences[i].size(); j++)
                cout << sequences[i][j] << " ";
            cout << endl;
        }
        cout << "END CORPUS ----------" << endl << endl << endl;
        cout << testGraph << endl;
        cout << "BEGIN DISTILLATION ----------" << endl;
        testGraph.distill(ADIOSParams(atof(argv[2]), atof(argv[3]), atoi(argv[4]), atof(argv[5])));
        cout << "END DISTILLATION ----------" << endl << endl;
        cout << testGraph << endl << endl;
        std::cout << endl << "Time elapsed: " << endTime - startTime << " seconds" << endl << endl << endl << endl;
        testGraph.convert2PCFG(std::cout);
    }
/*
    startTime = getTime();
    testGraph.distill(ADIOSParams(atof(argv[2]), atof(argv[3])*10, atoi(argv[4])-2, atof(argv[5])));
    endTime = getTime();
    cout << testGraph << endl << endl;

    std::cout << endl << "Time elapsed: " << endTime - startTime << " seconds" << endl << endl << endl << endl;*/
/*
    vector<string> testString(sequences[10].begin(), sequences[10].end());
    for(unsigned int i = 0; i < testString.size() - 1; i++)
        std::cout << testString[i] << " ";
    std::cout << testString.back() << endl;
    SearchPath newPath = testGraph.encode(testString);
    std::cout << newPath << endl;
    testGraph.predict(newPath);

    if(argc > 6)
        for(unsigned int i = 0; i < static_cast<unsigned int>(atoi(argv[6])); i++)
        {
            vector<string> sequence = testGraph.generate();
            for(unsigned int j = 0; j < sequence.size(); j++)
                std::cout << sequence[j] << " ";
            std::cout << endl;
        }*/
}
