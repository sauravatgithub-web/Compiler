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

        if(line.find("Non-terminating Symbols") != std::string::npos) {
            addNonTerminals(line);
        }
        else if(line.find("Terminating Symbols") != std::string::npos) {
            addTerminals(line);
        }
        else if(line.find("Productions") != std::string::npos) {
            while(std::getline(infile, line) && !line.empty() && line.find("Start Symbol") == std::string::npos) {
                createProductions(line);
            }
        }
    }

    startSymbol = symbolTable["START"];

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
}

void Grammar::addTerminals(const std::string& line) {
    std::string symbols = line.substr(line.find(":") + 1);
    std::stringstream ss(symbols);
    std::string sym;
    while(std::getline(ss, sym, ',')) {
        sym.erase(remove_if(sym.begin(), sym.end(), ::isspace), sym.end());
        if(!sym.empty()) {
            if(sym == "$") {
                terminals.push_back(EPSILON_SYMBOL);
                symbolTable[sym] = EPSILON_SYMBOL;
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
}

SymbolSet Grammar::makeFirstsUtility(Symbol sym) {
    if(!first[sym].empty()) return first[sym];
    
    for(Production prod : productions[sym]) {
        bool allNullable = true;

        int size = prod.size();
        for(int i = 0; i < size; i++) {
            SymbolSet firstTerms = makeFirstsUtility(prod[i]);
            for(Symbol s : firstTerms) {
                if(!s.isEpsilon()) first[sym].insert(s);
            }

            if(firstTerms.find(EPSILON_SYMBOL) == firstTerms.end()) {
                allNullable = false;
                break;
            }
        }

        if(!allNullable) first[sym].insert(EPSILON_SYMBOL);
    }

    return first[sym];
}

void Grammar::makeFirsts() {
    for(Symbol term_sm : terminals) {
        first[term_sm].insert(term_sm);
    }

    for(Symbol nonTerm_sym : nonTerminals) {
        if(!first[nonTerm_sym].empty()) continue;
        makeFirstsUtility(nonTerm_sym);
    }
}

void Grammar::makeFollows() {
    follow[startSymbol].insert(END_OF_INPUT_SYMBOL);

    bool changed = true;
    while(true) {
        changed = false;

        for(auto [sym, prods] : productions) {
            for(Production p : prods) {
                int size = p.size();

                for(int i = 0; i < size; i++) {
                    if(p[i].nature != Nature::NonTerminal) continue;

                    bool allNullable = true;
                    for(int j = i + 1; j < size; j++) {
                        for(Symbol s : first[p[j]]) {
                            if(!s.isEpsilon()) {
                                if(follow[p[i]].insert(s).second) changed = true;
                            }
                        }

                        if(first[p[j]].count(EPSILON_SYMBOL) == 0) {
                            allNullable = false;
                            break;
                        }
                    }

                    if(i == size - 1 || allNullable) {
                        for(Symbol s : follow[sym]) {
                            if(follow[p[i]].insert(s).second) changed = true;
                        }
                    }
                }
            }
        }
    }
}