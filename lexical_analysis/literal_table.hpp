#pragma once

#include <string>
#include <variant>
#include <vector>
#include <optional>
#include <unordered_map>

typedef std::variant<int, double, long long, long double, char, std::string, bool> Val;

enum class LiteralType {
    Integer, Floating, Character, String, Boolean
};

struct LiteralInfo {              
    std::string lexeme;           
    LiteralType type;             
    Val value;
    size_t length = 0;           

    LiteralInfo(std::string lx, LiteralType t, Val v)
        : lexeme(std::move(lx)), type(t), value(std::move(v)) {
        if(std::holds_alternative<std::string>(value)) {
            length = std::get<std::string>(value).size();
        }
    }
};

class LiteralTable {
private:
    std::unordered_map<std::string, int> table;
    std::vector<LiteralInfo> literals;
public:
    int addLiteral(const std::string& lexeme, LiteralType type, Val value) {
        if(table.find(lexeme) != table.end()) {
            return table[lexeme];
        }

        int idx = literals.size();
        literals.emplace_back(lexeme, type, value);
        table[lexeme] = idx;
        return idx;
    }

    const LiteralInfo* getLiteral(int index) const {
        if (index < 0 || index >= (int)literals.size()) return nullptr;
        return &literals[index];
    }

    const std::vector<LiteralInfo>& getAll() const {
        return literals;
    }
};