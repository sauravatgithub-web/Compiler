# pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "symbol.hpp"

typedef std::vector<Symbol> Production;
typedef std::unordered_set<Symbol, SymbolHash> SymbolSet;
typedef std::unordered_map<std::pair<Symbol, Symbol>, Production, SymbolPairHash> LL1_ParseTable;
typedef std::tuple<Symbol, int, int> Item;
typedef std::set<Item> LR_State;
typedef std::unordered_map<std::pair<int, Symbol>, int, SymbolIntHash> LR_Automation;
typedef std::map<LR_State, int> StateIndexMap;
typedef std::map<int, LR_State> IndexStateMap;

class Grammar {
protected:
    std::vector<Symbol> terminals;
    std::vector<Symbol> nonTerminals;
    std::unordered_map<std::string, Symbol> symbolTable;

public:
    Symbol startSymbol, preStartSymbol;
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
    LL1_ParseTable create_parse_table();
    bool LL1_parser(const std::vector<Token>& tokens);

    // std::vector<Item> create_items() {
    //     std::vector<Item> items;
    //     for(auto SymbolProductions : productions) {
    //         int length = SymbolProductions.second.size();
            
    //         for(int i = 0; i < length; i++)
    //     }
    // }
    void closure(LR_State& state);
    std::tuple<LR_Automation, StateIndexMap, IndexStateMap> createLR0Automation();
    bool LR0_parser(const std::vector<Token>& tokens);
};