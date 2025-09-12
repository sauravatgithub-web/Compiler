#include <set>
#include <map>
#include <queue>
#include "grammar.hpp"

LAStateIndexMap Grammar::create_LR1_states() {
    LAStateIndexMap stateMap;

    std::vector<Symbol> allSymbols;
    allSymbols.insert(allSymbols.end(), terminals.begin(), terminals.end());
    allSymbols.insert(allSymbols.end(), nonTerminals.begin(), nonTerminals.end());

    LALR_State start;
    start.insert({preStartSymbol, 0, 0, {END_OF_INPUT_SYMBOL}});
    closure(start);

    std::queue<LALR_State> q;
    stateMap[start] = 0;
    q.push(start);
    int nextIndex = 1;

    while(!q.empty()) {
        LALR_State currentState = q.front(); 
        q.pop();

        for(Symbol& X : allSymbols) {
            if(X == END_OF_INPUT_SYMBOL || X.isEpsilon()) continue;

            LALR_State nextState = GOTO(currentState, X);
            if(nextState.empty()) continue;

            if(!stateMap.count(nextState)) {
                stateMap[nextState] = nextIndex;
                q.push(nextState);
                nextIndex++;
            }
        }
    }

    return stateMap;
}

LR_ParseTable Grammar::create_LR1_parseTable() {
    LAStateIndexMap stateMap = create_LR1_states();
    LR_ParseTable parseTable;

    for(auto& [state, index] : stateMap) {
        for(const LAItem& item : state) {
            const auto& [sym, prodIdx, pos, firsts] = item;
            Production& production = productions[sym][prodIdx];

            if(pos != (int)production.size() && production[pos].nature == Nature::Terminal) {
                Symbol nextSym = production[pos];
                LALR_State nextState = GOTO(state, nextSym);
                if(!nextState.empty() && stateMap.count(nextState)) {
                    if(parseTable.count({index, nextSym})) {
                        std::cerr << "Sorry, Grammar is not LR(1)." << std::endl;
                        std::exit(EXIT_FAILURE);
                    }
                    parseTable[{index, nextSym}] = Action(ActionType::SHIFT, stateMap[nextState]);
                }
            }
            else if(pos == (int)production.size() && sym == preStartSymbol) {
                parseTable[{index, END_OF_INPUT_SYMBOL}] = Action(ActionType::ACCEPT);
            }
            else if(pos == (int)production.size()) {
                for(const Symbol& term : firsts) {
                    if(parseTable.count({index, term})) {
                        std::cerr << "Sorry, Grammar is not LR(1)." << std::endl;
                        std::exit(EXIT_FAILURE);
                    }
                    parseTable[{index, term}] = Action(ActionType::REDUCE, Item(sym, prodIdx, pos));
                }
            }
        }

        for(const Symbol& nonTerm : nonTerminals) {
            LALR_State nextState = GOTO(state, nonTerm);
            if(!nextState.empty() && stateMap.count(nextState)) {
                parseTable[{index, nonTerm}] = stateMap[nextState]; 
            }
        }
    }

    return parseTable;
}

bool Grammar::LR1_parser(const std::vector<Token>& tokens) {
    LR_ParseTable parseTable = create_LR1_parseTable();
    return LR_Parser(parseTable, tokens);
}