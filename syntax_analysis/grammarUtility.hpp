# pragma once

#include <vector>
#include <set>
#include <map>
#include <variant>
#include <unordered_set>
#include <unordered_map>
#include "symbol.hpp"

typedef std::vector<Symbol> Production;
typedef std::unordered_set<Symbol, SymbolHash> SymbolSet;
typedef std::unordered_map<std::pair<Symbol, Symbol>, Production, SymbolPairHash> LL1_ParseTable;
typedef std::tuple<Symbol, int, int> Item;
typedef std::set<Item> LR_State;
typedef std::unordered_map<std::pair<int, Symbol>, int, SymbolIntHash> LR_Automation;
typedef std::map<LR_State, int> StateIndexMap;
typedef std::map<int, LR_State> IndexStateMap;
typedef std::variant<std::monostate, Action, int> parseAction;
typedef std::unordered_map<std::pair<int, Symbol>, parseAction, SymbolIntHash> LR_ParseTable;

enum class ActionType { SHIFT, REDUCE, ACCEPT };

class Action {
public:
    ActionType type;
    int stateIndex;
    Item item;

    Action(ActionType type) : type(type) {}
    Action(ActionType type, int index) : type(type), stateIndex(index) {}
    Action(ActionType type, Item item) : type(type), item(item) {}
};