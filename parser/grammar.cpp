#include <sstream>
#include <algorithm>
#include "grammar.hpp"

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
}

void Grammar::addTerminals(const std::string& line) {
    std::string symbols = line.substr(line.find(":") + 1);
    std::stringstream ss(symbols);
    std::string sym;
    while(std::getline(ss, sym, ',')) {
        sym.erase(remove_if(sym.begin(), sym.end(), ::isspace), sym.end());
        if(!sym.empty()) {
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
}