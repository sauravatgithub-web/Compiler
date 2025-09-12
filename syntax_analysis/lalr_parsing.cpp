#include <set>
#include <map>
#include <queue>
#include "grammar.hpp"

MergedStates Grammar::mergeStates(const LAStateIndexMap& orgMap) {
    LAStateIndexMap newMap;
    CoreStateMap coreMap;
    StateCoreMap stateMap;
    std::map<LR_State, LALR_State> itemMaps;

    auto merge = [&](LR_State core, LALR_State s2) {
        LALR_State s1 = itemMaps[core];

        std::map<Item, std::set<Symbol>> laMap;
        for(auto& [sym, prodIdx, pos, firsts] : s1) {
            laMap[Item(sym, prodIdx, pos)].insert(firsts.begin(), firsts.end());
        }
        for(auto& [sym, prodIdx, pos, firsts] : s2) {
            laMap[Item(sym, prodIdx, pos)].insert(firsts.begin(), firsts.end());
        }

        LALR_State merged;
        for(auto& [item, las] : laMap) {
            auto& [sym, prodIdx, pos] = item;
            merged.insert(LAItem(sym, prodIdx, pos, las));
        }
        itemMaps[core] = merged;

        return merged;
    };

    for(auto [state, index] : orgMap) {
        LR_State core;
        for(auto [sym, prodIdx, pos, firsts] : state) {
            core.insert(Item(sym, prodIdx, pos));
        }
        if(!itemMaps.count(core)) itemMaps[core] = state;
        else itemMaps[core] = merge(core, state);
    }

    int index = 0;
    for(auto& [core, merged] : itemMaps) {
        newMap[merged] = index++;
        coreMap[core] = merged;
        stateMap[merged] = core;
    }

    return {coreMap, stateMap, newMap};
}

LR_ParseTable Grammar::create_LALR_parseTable() {
    LAStateIndexMap orgMap = create_LR1_states();
    auto [coreToStateMap, stateToCoreMap, stateMap] = mergeStates(orgMap);
    LR_ParseTable parseTable;

    for(auto& [state, index] : stateMap) {
        for(const LAItem& item : state) {
            const auto& [sym, prodIdx, pos, firsts] = item;
            Production& production = productions[sym][prodIdx];

            if(pos != (int)production.size() && production[pos].nature == Nature::Terminal) {
                Symbol nextSym = production[pos];
                LR_State nextState = GOTO(stateToCoreMap[state], nextSym);
                if(!nextState.empty() && coreToStateMap.count(nextState)) {
                    if(parseTable.count({index, nextSym})) {
                        std::cerr << "Sorry, Grammar is not LALR." << std::endl;
                        std::exit(EXIT_FAILURE);
                    }
                    parseTable[{index, nextSym}] = Action(ActionType::SHIFT, stateMap[coreToStateMap[nextState]]);
                }
            }
            else if(pos == (int)production.size() && sym == preStartSymbol) {
                parseTable[{index, END_OF_INPUT_SYMBOL}] = Action(ActionType::ACCEPT);
            }
            else if(pos == (int)production.size()) {
                for(const Symbol& term : firsts) {
                    if(parseTable.count({index, term})) {
                        std::cerr << "Sorry, Grammar is not LALR." << std::endl;
                        std::exit(EXIT_FAILURE);
                    }
                    parseTable[{index, term}] = Action(ActionType::REDUCE, Item(sym, prodIdx, pos));
                }
            }
        }

        for(const Symbol& nonTerm : nonTerminals) {
            LR_State nextState = GOTO(stateToCoreMap[state], nonTerm);
            if(!nextState.empty() && coreToStateMap.count(nextState)) {
                parseTable[{index, nonTerm}] = stateMap[coreToStateMap[nextState]]; 
            }
        }
    }

    return parseTable;
}

bool Grammar::LALR_parser(const std::vector<Token>& tokens) {
    LR_ParseTable parseTable = create_LALR_parseTable();
    return LR_Parser(parseTable, tokens);
}