# pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "symbol.hpp"

typedef std::vector<Symbol> Production;
typedef std::unordered_set<Symbol, SymbolHash> SymbolSet;
typedef std::unordered_map<std::pair<Symbol, Symbol>, Production, SymbolPairHash> ParseTable;

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

    void print() {
        std::cout << "hello from grammar" << std::endl;
        for(auto s : terminals) std::cout << s.name << std::endl;
    }

    bool recursive_descent_parser(const std::vector<Token>& tokens);
    std::vector<int> recursive_descent_main_procedure(const Symbol& sym, const std::vector<Token>& tokens, int index);
    std::vector<int> recursive_descent_procedure(const Production& prod, const std::vector<Token> tokens, int index);

    void makeFirsts();
    SymbolSet makeFirstsUtility(Symbol sym);
    void makeFollows();
    ParseTable create_parse_table();
    bool LL1_parser(const std::vector<Token>& tokens);
};
