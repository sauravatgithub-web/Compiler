#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "grammar.hpp"
#include "../definitions.hpp"

int Grammar::create() {
    std::ifstream infile(GRAMMAR_SOURCE_FILE);
    if(!infile.is_open()) {
        std::cerr << "Failed to open grammar source file" << std::endl;
        return -1;
    }

    std::string line;
    while(std::getline(infile, line)) {
        if(line.empty()) continue;

        if(line.find("NON-TERMINATING SYMBOLS") != std::string::npos) {
            addNonTerminals(line);
        }
        else if(line.find("TERMINATING SYMBOLS") != std::string::npos) {
            addTerminals(line);
        }
        else if(line.find("PRODUCTIONS") != std::string::npos) {
            while(std::getline(infile, line) && line.find("START SYMBOL") == std::string::npos) {
                if(line.empty()) continue;
                createProductions(line);
            }
        }
    }

    startSymbol = symbolTable["START"];
    preStartSymbol = symbolTable["PRE_START"];

    return 0;
}

void Grammar::addNonTerminals(const std::string& line) {
    std::string symbols = line.substr(line.find(":") + 1);
    std::stringstream ss(symbols);
    std::string sym;
    while(std::getline(ss, sym, ',')) {
        sym.erase(remove_if(sym.begin(), sym.end(), ::isspace), sym.end());
        if(!sym.empty()) {
            Symbol symbol = Symbol(sym, false);
            nonTerminals.push_back(symbol);
            symbolTable[sym] = symbol;
        }
    }

    Symbol pre_start = Symbol("PRE_START", false);
    nonTerminals.push_back(pre_start);
    symbolTable["PRE_START"] = pre_start;
}

void Grammar::addTerminals(const std::string& line) {
    symbolTable[EPSILON] = EPSILON_SYMBOL;
    symbolTable[END_OF_INPUT] = END_OF_INPUT_SYMBOL;
    terminals.push_back(EPSILON_SYMBOL);
    terminals.push_back(END_OF_INPUT_SYMBOL);

    std::string symbols = line.substr(line.find(":") + 1);
    std::stringstream ss(symbols);
    std::string sym;
    while(std::getline(ss, sym, ',')) {
        sym.erase(remove_if(sym.begin(), sym.end(), ::isspace), sym.end());
        if(!sym.empty()) {
            if(sym == EPSILON) {
                terminals.push_back(EPSILON_SYMBOL);
                continue;
            }
            Symbol symbol = Symbol(sym, true);
            terminals.push_back(symbol);
            symbolTable[sym] = symbol;
        }
    }
}

void Grammar::createProductions(const std::string& line) {
    std::stringstream ss(line);
    std::string lhs, arrow, rhs;
    ss >> lhs >> arrow;
    std::getline(ss, rhs);

    Symbol leftSymbol = symbolTable[lhs];

    std::stringstream ssRhs(rhs);
    std::string alternative;
    while(std::getline(ssRhs, alternative, '|')) {
        std::stringstream ssAlt(alternative);
        std::string token;
        Production production;
        while(ssAlt >> token) {
            production.push_back(symbolTable[token]); 
        }
        productions[leftSymbol].push_back(production);
    }

    productions[symbolTable["PRE_START"]].push_back({symbolTable["START"]});
}
