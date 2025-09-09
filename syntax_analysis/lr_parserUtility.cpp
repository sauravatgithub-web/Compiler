#include <stack>
#include <queue>
#include "grammar.hpp"

void Grammar::closure(LR_State& state) {
    std::queue<Item> q;
    for(const auto& it : state) q.push(it);

    while(!q.empty()) {
        Item item = q.front(); 
        q.pop();
        auto& [sym, prodIdx, pos] = item;

        if(pos < (int)productions[sym][prodIdx].size()) {
            Symbol nextSym = productions[sym][prodIdx][pos];

            if(nextSym.nature == Nature::NonTerminal) {
                for(int i = 0; i < (int)productions[nextSym].size(); i++) {
                    Item newItem = {nextSym, i, 0};

                    if(!state.count(newItem)) {
                        state.insert(newItem);
                        q.push(newItem);
                    }
                }
            }
        }
    }
}

LR_State Grammar::GOTO(LR_State state, Symbol sym) {
    LR_State nextState;

    for(auto [symbol, index, pos] : state) {
        if(pos != (int)productions[symbol][index].size() && productions[symbol][index][pos] == sym) {
            nextState.insert({symbol, index, pos+1});
        }
    }

    closure(nextState);
    return nextState;
}

bool Grammar::LR_Parser(LR_ParseTable parseTable, const std::vector<Token>& tokens) {
    std::stack<int> parserStack;
    parserStack.push(0);
    int index = 0;

    while(true) {
        int state = parserStack.top();
        Symbol sym = symbolTable[tokenName[tokens[index].type]];
        std::cout << sym.name << std::endl;

        auto it = parseTable.find({state, sym});
        if(it == parseTable.end()) {
            std::cerr << "Parse error at state " << state << " on symbol " << sym.name << "\n";
            return false;
        }

        const ParseAction& entry = it->second;

        if(std::holds_alternative<Action>(entry)) {
            const Action& act = std::get<Action>(entry);

            if(act.type == ActionType::SHIFT) {
                parserStack.push(act.stateIndex);
                index++;
            }
            else if(act.type == ActionType::REDUCE) {
                const auto& [symbol, prodIdx, pos] = act.item;
                int length = productions[symbol][prodIdx].size();

                for(int i = 1; i <= length; i++) {
                    parserStack.pop();
                }

                int newState = parserStack.top();
                auto gotoIt = parseTable.find({newState, symbol});
                if(gotoIt == parseTable.end() || !std::holds_alternative<int>(gotoIt->second)) {
                    return false;
                }

                int gotoState = std::get<int>(gotoIt->second);
                parserStack.push(gotoState);
            }
            else if(act.type == ActionType::ACCEPT) {
                return true;
            }
            else return false;
        }
        else if(std::holds_alternative<int>(entry)) {
            parserStack.push(std::get<int>(entry));
        }
        else return false;
    }
}