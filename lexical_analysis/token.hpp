#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <variant>
#include "symbol_table.hpp"
#include "literal_table.hpp"

enum class token_type { 
    _integer, _id, _if, _then, _else, _relop, _int, _float, _bool, _string, _char, _double, _vector, _set, _map,
    _while, _std, _cout, _cin, _aop, _bop, _lop, _assignop, _end, _return, _fas, _fae, _cbs, _cbe, _scrop, _comma,
    _uset, _umap, _int_lit, _doub_lit, _char_lit, _str_lit, _main, _long, _dollar, _larrow, _rarrow, _dlarrow, _drarrow, _endl
};

class Token {
public: 
    token_type type;
    std::string lexeme;

    typedef std::variant<std::monostate, int, double, long long, long double, std::string, char, bool> ValueType;
    ValueType value = std::monostate{};

    int symbolTableIndex = -1;
    int literalTableIndex = -1;

    Token() = default;
    Token(token_type t, const std::string l) : type(t), lexeme(l) {}

    template<typename T>
    Token(token_type t, const std::string& l, T val) : type(t), lexeme(l), value(val) {}
};

void token_creator(const std::string& chars, const std::string machine, SymbolTable& symTable, LiteralTable& litTable, std::vector<Token>& token_sequence);

std::ostream& operator<<(std::ostream& os, const token_type& t);
std::ostream& operator<<(std::ostream& os, const Token& token);