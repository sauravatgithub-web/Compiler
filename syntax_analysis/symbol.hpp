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

inline const Symbol EPSILON_SYMBOL(EPSILON, true);
inline const Symbol END_OF_INPUT_SYMBOL(END_OF_INPUT, true);