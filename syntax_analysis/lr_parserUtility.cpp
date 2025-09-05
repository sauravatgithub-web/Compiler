#include <stack>
#include "grammar.hpp"

void Grammar::closure(LR_State& state) {
    bool changed = true;
    while(changed) {
        changed = false;

        for(auto& [symbol, index, pos] : state) {
            if(pos != productions[symbol][index].size()) {
                Symbol nextSym = productions[symbol][index][pos];

                if(nextSym.nature == Nature::NonTerminal) {
                    int size = productions[nextSym].size();

                    for(int i = 0; i < size; i++) {
                        Item newItem = {nextSym, i, 0};

                        if(!state.count(newItem)) {
                            changed = true;
                            state.insert(newItem);
                        }
                    }
                }
            }
        }
    }
}

LR_State Grammar::GOTO(LR_State state, Symbol sym) {
    LR_State nextState;

    for(auto [symbol, index, pos] : state) {
        if(pos != productions[symbol][index].size() && productions[symbol][index][pos] == sym) {
            nextState.insert({symbol, index, pos+1});
        }
    }

    closure(nextState);
    return nextState;
}

bool Grammar::LR_Parser(LR_ParseTable parseTable, const std::vector<Token>& tokens) {
    std::stack<int> parserStack;
    std::vector<Symbol> symbols;

    parserStack.push(0);
    int index = 0;

    while(true) {
        int state = parserStack.top();
        Symbol sym = symbolTable[tokenName[tokens[index].type]];

        if(sym.nature == Nature::NonTerminal) {
            parserStack.push(parseTable[{state, sym}]);
        }
        else {
            if(parseTable[{state, sym}].type == ActionType::SHIFT) {
                parserStack.push(parseTable[{state, sym}].state);
            }
            else if(parseTable[{state, sym}].type == ActionType::REDUCE) {
                auto [symbol, prodIdx, pos] = parseTable[{state, sym}].item;

                int length = productions[symbol][prodIdx].size();
                for(int i = 1; i <= length; i++) {
                    parserStack.pop();
                }

                int newState = parserStack.top();
                parserStack.push(parseTable[{newState, symbol}]);
            }
            else if(parseTable[{state, sym}].type == ActionType::ACCEPT) {
                return true;
            }
            else return false;
        }
    }
}