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
    std::unordered_map<Symbol, std::vector<std::vector<Symbol>>, SymbolHash> productions;
    std::unordered_map<std::string, Symbol> symbolTable;
public:
    Symbol startSymbol;

    void addNonTerminals(const std::string& line);
    void addTerminals(const std::string& line);
    void createProductions(const std::string& line);
};