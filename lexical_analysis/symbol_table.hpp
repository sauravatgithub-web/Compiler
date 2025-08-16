# pragma once

#include <string>
#include <vector>
#include <unordered_map>

struct SymbolInfo {
    std::string name;
    std::string type;
    int scopeLevel;
};

class SymbolTable {
private:
    std::unordered_map<std::string, int> table;  
    std::vector<SymbolInfo> symbols;             
public:
    int addSymbol(const std::string &name) {
        if(table.find(name) != table.end()) {
            return table[name];
        }

        int idx = symbols.size();
        symbols.push_back({name, "", 0});
        table[name] = idx;
        return idx;
    }

    SymbolInfo* lookup(const std::string &name) {
        auto it = table.find(name);
        if(it != table.end()) return &symbols[it->second];
        return nullptr;
    }

    int getIndex(const std::string &name) {
        auto it = table.find(name);
        return (it != table.end()) ? it->second : -1;
    }
};