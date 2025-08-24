# pragma once

#include <string>
#include "../definitions.hpp"

enum class Nature { Terminal, NonTerminal };

class Symbol {
public:
    std::string name;
    Nature nature;
    
    Symbol() = default;
    Symbol(std::string sym, bool isTerminal) 
        : name(sym), nature(isTerminal ? Nature::Terminal : Nature::NonTerminal) {}

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

inline const Symbol EPSILON_SYMBOL(EPSILON, true);
inline const Symbol END_OF_INPUT_SYMBOL(END_OF_INPUT, true);