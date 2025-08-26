#include <stack>
#include "grammar.hpp"

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

ParseTable Grammar::create_parse_table() {
    ParseTable parseTable;

    for(auto [sym, symbolProductions] : productions) {
        if(sym.nature == Nature::Terminal) continue;

        for(Production p : symbolProductions) {
            if(!p[0].isEpsilon()) {
                if(p[0].nature == Nature::Terminal) parseTable[{sym, p[0]}] = p;
                else {
                    for(Symbol nonTerm_sym : first[p[0]]) {
                        parseTable[{sym, nonTerm_sym}] = p;
                    }
                }
            }
            else {
                for(Symbol term_sym : follow[sym]) {
                    parseTable[{sym, term_sym}] = p;
                }
            }
        }
    }

    return parseTable;
}

bool Grammar::LL1_parser(const std::vector<Token>& tokens) {
    ParseTable parseTable = create_parse_table();

    std::stack<Symbol> parserStack;
    parserStack.push(startSymbol);
}