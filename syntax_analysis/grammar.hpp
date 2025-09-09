# pragma once

#include "grammarUtility.hpp"

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

    void closure(LR_State& state);
    LR_State GOTO(LR_State state, Symbol sym);
    bool LR_Parser(LR_ParseTable parseTable, const std::vector<Token>& tokens);

    StateIndexMap create_LR0_states();
    LR_ParseTable create_LR0_parseTable();
    bool LR0_parser(const std::vector<Token>& tokens);
    
    StateIndexMap create_SLR1_states();
    LR_ParseTable create_SLR1_parseTable();
    bool SLR1_parser(const std::vector<Token>& tokens);
};