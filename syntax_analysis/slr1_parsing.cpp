#include <set>
#include <map>
#include <queue>
#include "grammar.hpp"

StateIndexMap Grammar::create_SLR1_states() {
    StateIndexMap stateMap;
    IndexStateMap indexToState;

    std::vector<Symbol> allSymbols;
    allSymbols.insert(allSymbols.end(), terminals.begin(), terminals.end());
    allSymbols.insert(allSymbols.end(), nonTerminals.begin(), nonTerminals.end());

    LR_State start;
    start.insert({preStartSymbol, 0, 0});
    closure(start);

    std::queue<LR_State> q;
    stateMap[start] = 0;
    indexToState[0] = start;
    q.push(start);
    int nextIndex = 1;

    while(!q.empty()) {
        LR_State currentState = q.front(); 
        q.pop();

        for(const Symbol& X : allSymbols) {
            LR_State nextState = GOTO(currentState, X);
            if(nextState.empty()) continue;

            if(!stateMap.count(nextState)) {
                stateMap[nextState] = nextIndex;
                indexToState[nextIndex] = nextState;
                q.push(nextState);
                nextIndex++;
            }
        }
    }

    return stateMap;
}

LR_ParseTable Grammar::create_SLR1_parseTable() {
    StateIndexMap stateMap = create_SLR1_states();
    LR_ParseTable parseTable;

    for(auto& [state, index] : stateMap) {
        for(const Item& item : state) {
            const auto& [sym, prodIdx, pos] = item;
            const Production& production = productions[sym][prodIdx];

            if(pos != (int)production.size() && production[pos].nature == Nature::Terminal) {
                Symbol nextSym = production[pos];
                LR_State nextState = GOTO(state, nextSym);
                if(!nextState.empty() && stateMap.count(nextState)) {
                    if(parseTable.count({index, nextSym})) {
                        std::cerr << "Sorry, Grammar is not SLR(1)." << std::endl;
                        std::exit(EXIT_FAILURE);
                    }
                    parseTable[{index, nextSym}] = Action(ActionType::SHIFT, stateMap[nextState]);
                }
            }
            else if(pos == (int)production.size() && sym == preStartSymbol) {
                parseTable[{index, END_OF_INPUT_SYMBOL}] = Action(ActionType::ACCEPT);
            }
            else if(pos == (int)production.size()) {
                for(const Symbol& term : follow[sym]) {
                    if(parseTable.count({index, term})) {
                        std::cerr << "Sorry, Grammar is not SLR(1)." << std::endl;
                        std::exit(EXIT_FAILURE);
                    }
                    parseTable[{index, term}] = Action(ActionType::REDUCE, item);
                }
            }
        }

        for(const Symbol& nonTerm : nonTerminals) {
            LR_State nextState = GOTO(state, nonTerm);
            if(!nextState.empty() && stateMap.count(nextState)) {
                if(parseTable.count({index, nonTerm})) {
                    std::cerr << "Sorry, Grammar is not SLR(1)." << std::endl;
                    std::exit(EXIT_FAILURE);
                }
                parseTable[{index, nonTerm}] = stateMap[nextState]; 
            }
        }
    }

    return parseTable;
}


bool Grammar::SLR1_parser(const std::vector<Token>& tokens) {
    LR_ParseTable parseTable = create_SLR1_parseTable();
    return LR_Parser(parseTable, tokens);
}