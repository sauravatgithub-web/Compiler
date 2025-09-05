#include <set>
#include <map>
#include <stack>
#include <queue>
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

std::tuple<LR_Automation, StateIndexMap, IndexStateMap> Grammar::createLR0Automation() {
    LR_Automation LR0_Automation;
    StateIndexMap stateIndex;
    IndexStateMap indexState;
    std::queue<int> stateQueue;

    int index = 0;
    LR_State state;
    state.insert({preStartSymbol, 0, 0});
    closure(state);
    stateIndex[state] = index;
    indexState[index] = state;
    stateQueue.push(index);

    while(!stateQueue.empty()) {
        int idx = stateQueue.front();
        stateQueue.pop();

        LR_State currentState = indexState[idx];

        auto GOTO = [&](Symbol sym) {
            LR_State nextState;

            for(auto [symbol, index, pos] : currentState) {
                if(pos != productions[symbol][index].size() && productions[symbol][index][pos] == sym) {
                    nextState.insert({symbol, index, pos+1});
                }
            }

            closure(nextState);
            if(!stateIndex.count(nextState)) {
                index++;
                stateIndex[nextState] = index;
                indexState[index] = nextState;
                stateQueue.push(index);
                LR0_Automation[{stateIndex[currentState], sym}] = index;
            }
        };

        for(Symbol term : terminals) {
            GOTO(term);
        }
        for(Symbol nonTerm : nonTerminals) {
            GOTO(nonTerm);
        }
    }

    return {LR0_Automation, stateIndex, indexState};
}


bool Grammar::LR0_parser(const std::vector<Token>& tokens) {
    auto [LR0_Automation, stateMap, indexMap] = createLR0Automation();

    std::stack<int> parserStack;
    std::vector<Symbol> symbols;

    parserStack.push(0);
    symbols.push_back(END_OF_INPUT_SYMBOL);
    int index = 0;

    while(true) {
        int state = parserStack.top();
        Symbol sym = symbolTable[tokenName[tokens[index].type]];

        if(LR0_Automation.count({state, sym})) {
            parserStack.push(LR0_Automation[{state, sym}]);
            symbols.push_back(sym);
            index++;
        }
        else {
            LR_State automationState = indexMap[state];
            Symbol nextSym;
            int count = 0;

            for(auto [sym, prodIdx, pos] : automationState) {
                if(pos == productions[sym][prodIdx].size()) {
                    nextSym = sym;
                    count = pos;
                    break;
                }
            }

            if(nextSym == preStartSymbol) return true;
            if(count == 0) return false;

            for(int i = 1; i <= count; i++) {
                parserStack.pop();
                symbols.pop_back();
            }

            symbols.push_back(nextSym);
            state = parserStack.top();
            parserStack.push(LR0_Automation[{state, nextSym}]);
        }
    }
}