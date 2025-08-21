#include <iostream>
#include <fstream>
#include "grammar.hpp"
#include "../definitions.hpp"

int main() {
    std::ifstream infile(GRAMMAR_SOURCE_FILE);
    if(!infile.is_open()) {
        std::cerr << "Failed to open grammar source file" << std::endl;
        return 1;
    }

    std::string line;
    while(std::getline(infile, line)) {
        if(line.empty()) continue;
        Grammar grammar;

        if(line.find("Non-terminating Symbols") != std::string::npos) {
            grammar.addNonTerminals(line);
        }
        else if(line.find("Terminating Symbols") != std::string::npos) {
            grammar.addTerminals(line);
        }
        else if(line.find("Productions") != std::string::npos) {
            while(std::getline(infile, line) && !line.empty() && line.find("Start Symbol") == std::string::npos) {
                grammar.createProductions(line);
            }
        }
    }



    return 0;
}