#include <set>
#include <map>
#include <queue>
#include "grammar.hpp"

StateIndexMap Grammar::create_SLR1_states() {

    auto create_items = [&]() {
        std::vector<Item> items;
        items.push_back({preStartSymbol, 0, 0});

        for(auto SymbolProductions : productions) {
            Symbol sym = SymbolProductions.first;
            int length = SymbolProductions.second.size();
            
            for(int i = 0; i < length; i++) {
                Production p = SymbolProductions.second[i];
                int size = p.size();

                for(int j = 0; j <= size; j++) {
                    items.push_back({sym, i, j});
                }
            }
        }

        return items;
    };


    std::vector<Item> items = create_items();
    std::map<LR_State, int> stateMap;
    int index = 0;

    for(Item item : items) {
        LR_State state;
        state.insert(item);
        closure(state);

        if(!stateMap.count(state)) {
            stateMap[state] = index++;
        }
    }

    return stateMap;
}

LR_ParseTable Grammar::create_SLR1_parseTable() {
    StateIndexMap stateMap = create_SLR1_states();
    LR_ParseTable parseTable;

    for(auto [state, index] : stateMap) {
        for(Item item : state) {
            auto [sym, prodIdx, pos] = item;
            Production production = productions[sym][prodIdx];

            if(pos != production.size() && production[pos].nature == Nature::Terminal) {
                LR_State nextState = GOTO(state, sym);
                parseTable[{index, sym}] = Action(ActionType::SHIFT, stateMap[nextState]);
            }
            else if(pos == production.size() && sym == preStartSymbol) {
                parseTable[{index, sym}] = Action(ActionType::ACCEPT);
            }
            else if(pos == production.size()) {
                for(Symbol terms : follow[sym]) {
                    parseTable[{index, sym}] = Action(ActionType::REDUCE, item);
                }
            }
        }

        for(Symbol nonTerm : nonTerminals) {
            parseTable[{index, nonTerm}] = stateMap[GOTO(state, nonTerm)];
        }
    }

    return parseTable;
}


bool Grammar::SLR1_parser(const std::vector<Token>& tokens) {
    LR_ParseTable parseTable = create_SLR1_parseTable();
    return LR_Parser(parseTable, tokens);
}