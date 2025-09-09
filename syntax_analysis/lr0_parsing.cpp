#include <map>
#include <stack>
#include <queue>
#include "grammar.hpp"

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

            for(auto [symbol, prodIdx, pos] : currentState) {
                if(pos != (int)productions[symbol][prodIdx].size() && productions[symbol][prodIdx][pos] == sym) {
                    nextState.insert({symbol, prodIdx, pos+1});
                }
            }

            if(nextState.empty()) return;

            closure(nextState);
            if(!stateIndex.count(nextState)) {
                index++;
                stateIndex[nextState] = index;
                indexState[index] = nextState;
                stateQueue.push(index);
            }

            LR0_Automation[{stateIndex[currentState], sym}] = stateIndex[nextState];
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
        if(index >= (int)tokens.size()) return false;

        int state = parserStack.top();
        Symbol sym = (index < (int)tokens.size()) ? symbolTable[tokenName[tokens[index].type]] : END_OF_INPUT_SYMBOL;

        if(LR0_Automation.count({state, sym})) {
            parserStack.push(LR0_Automation[{state, sym}]);
            symbols.push_back(sym);
            index++;
        }
        else {
            LR_State automationState = indexMap[state];
            Symbol nextSym;
            int count = 0;

            for(auto [newSym, prodIdx, pos] : automationState) {
                if(pos == (int)productions[newSym][prodIdx].size()) {
                    nextSym = newSym;
                    count = pos;
                    break;
                }
            }

            if(nextSym == preStartSymbol && sym == END_OF_INPUT_SYMBOL) return true;
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