# pragma once

#include <string>
#include "symbolMap.hpp"
#include "../definitions.hpp"

enum class Nature { Terminal, NonTerminal };

class Symbol {
public:
    std::string name;
    Nature nature;
    token_type type;
    
    Symbol() = default;
    Symbol(std::string sym, bool isTerminal) 
        : name(sym), nature(isTerminal ? Nature::Terminal : Nature::NonTerminal) {
            if(isTerminal) type = nameType[sym];
        }

    bool operator<(const Symbol& other) const {
        if(nature != other.nature) return nature < other.nature;
        return name < other.name;
    }

    bool operator==(const Symbol& other) const {
        return name == other.name && nature == other.nature;
    }

    inline bool isEpsilon() { return name == EPSILON; }
};

struct SymbolHash {
    std::size_t operator()(const Symbol& s) const {
        return std::hash<std::string>()(s.name) ^ (std::hash<int>()(static_cast<int>(s.nature)) << 1);
    }
};

struct SymbolPairHash {
    std::size_t operator()(const std::pair<Symbol, Symbol>& spair) const {
        std::size_t hashFirst = std::hash<std::string>()(spair.first.name) ^ (std::hash<int>()(static_cast<int>(spair.first.nature)) << 1);
        std::size_t hashSecond = std::hash<std::string>()(spair.second.name) ^ (std::hash<int>()(static_cast<int>(spair.second.nature)) << 1);
        return hashFirst ^ hashSecond;
    }
};

struct SymbolIntHash {
    std::size_t operator()(const std::pair<int, Symbol>& spair) const {
        std::size_t hash = std::hash<std::string>()(spair.second.name) ^ (std::hash<int>()(static_cast<int>(spair.second.nature)) << 1);
        return spair.first ^ hash;
    }
};

struct ItemHash {
    std::size_t operator()(const std::tuple<Symbol, int, int>& item) const {
        auto& [sym, idx, pos] = item;
        std::size_t h1 = std::hash<std::string>()(sym.name) ^ (std::hash<int>()(static_cast<int>(sym.nature)) << 1);
        std::size_t h2 = std::hash<int>()(idx);
        std::size_t h3 = std::hash<int>()(pos);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

struct LAItemHash {
    std::size_t operator()(const std::tuple<Symbol, int, int, std::set<Symbol>>& item) const {
        auto& [sym, idx, pos, las] = item;
        std::size_t h1 = std::hash<std::string>()(sym.name) ^ (std::hash<int>()(static_cast<int>(sym.nature)) << 1);
        std::size_t h2 = std::hash<int>()(idx);
        std::size_t h3 = std::hash<int>()(pos);
        std::size_t h4 = 0;
        for(const Symbol& s : las) {
            h4 ^= std::hash<std::string>()(s.name) ^ (std::hash<int>()(static_cast<int>(s.nature)) << 1);
        }
        return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
    }
};

inline const Symbol EPSILON_SYMBOL(EPSILON, true);
inline const Symbol END_OF_INPUT_SYMBOL(END_OF_INPUT, true);