# pragma once

#include <vector>
#include <unordered_map>
#include "symbol.hpp"

using Production = std::vector<Symbol>;
using SymbolProduction = std::vector<Production>;

class Grammar {
private:
    std::vector<Symbol> terminals;
    std::vector<Symbol> nonTerminals;
    std::unordered_map<std::string, Symbol> symbolTable;
public:
    Symbol startSymbol;
    std::unordered_map<Symbol, std::vector<Production>, SymbolHash> productions;

    int create();
    void addNonTerminals(const std::string& line);
    void addTerminals(const std::string& line);
    void createProductions(const std::string& line);
};