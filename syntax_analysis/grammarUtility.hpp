# pragma once

#include <vector>
#include <set>
#include <variant>
#include <unordered_set>
#include <unordered_map>
#include "symbol.hpp"

typedef std::vector<Symbol> Production;
typedef std::unordered_set<Symbol, SymbolHash> SymbolSet;
typedef std::unordered_map<std::pair<Symbol, Symbol>, Production, SymbolPairHash> LL1_ParseTable;
typedef std::tuple<Symbol, int, int> Item;
typedef std::tuple<Symbol, int, int, std::set<Symbol>> LAItem;
typedef std::set<Item> LR_State;
typedef std::set<LAItem> LALR_State;

struct StateHash {
    std::size_t operator()(const LR_State& state) const {
        std::size_t seed = 0;
        for (const auto& item : state) {
            seed ^= ItemHash{}(item) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

struct LAStateHash {
    std::size_t operator()(const LALR_State& state) const {
        std::size_t seed = 0;
        for (const auto& item : state) {
            seed ^= LAItemHash{}(item) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

typedef std::unordered_map<LR_State, int, StateHash> StateIndexMap;
typedef std::unordered_map<int, LR_State> IndexStateMap;
typedef std::unordered_map<LALR_State, int, LAStateHash> LAStateIndexMap;
typedef std::unordered_map<int, LALR_State> IndexLAStateMap;

enum class ActionType { SHIFT, REDUCE, ACCEPT };

class Action {
public:
    ActionType type;
    int stateIndex = -1;
    Item item = {};

    Action(ActionType type) : type(type) {}
    Action(ActionType type, int index) : type(type), stateIndex(index) {}
    Action(ActionType type, Item item) : type(type), item(item) {}
};

typedef std::variant<std::monostate, Action, int> ParseAction;
typedef std::unordered_map<std::pair<int, Symbol>, ParseAction, SymbolIntHash> LR_ParseTable;