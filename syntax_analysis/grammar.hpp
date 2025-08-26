# pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "symbol.hpp"
#include "lexical_analysis/token.hpp"

using Production = std::vector<Symbol>;
using SymbolSet = std::unordered_set<Symbol, SymbolHash>;

class Grammar {
private:
    std::vector<Symbol> terminals;
    std::vector<Symbol> nonTerminals;
    std::unordered_map<std::string, Symbol> symbolTable;
public:
    Symbol startSymbol;
    std::unordered_map<Symbol, std::vector<Production>, SymbolHash> productions;
    std::unordered_map<Symbol, SymbolSet, SymbolHash> first;
    std::unordered_map<Symbol, SymbolSet, SymbolHash> follow;

    int create();
    void addNonTerminals(const std::string& line);
    void addTerminals(const std::string& line);
    void createProductions(const std::string& line);

    bool recursive_descent_parser(const std::vector<Token>& tokens);
    std::vector<int> recursive_descent_main_procedure(const Symbol& sym, const std::vector<Token>& tokens, int index);
    std::vector<int> recursive_descent_procedure(const Production& prod, const std::vector<Token> tokens, int index);

    void makeFirsts();
    SymbolSet makeFirstsUtility(Symbol sym);
    void makeFollows();
};