#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <variant>
#include <unordered_map>
#include <unordered_set>
#include "symbol_table.hpp"
#include "literal_table.hpp"

enum token_type { 
    _integer, _id, _if, _then, _else, _relop, _int, _float, _bool, _string, _char, _double, _vector, _set, _map,
    _while, _std, _cout, _cin, _aop, _bop, _lop, _assign, _end, _return, _fas, _fae, _cbs, _cbe, _scrop, _comma,
    _uset, _umap
};

std::unordered_set<std::string> keywords = { 
    "if", "then", "else", "int", "float", "bool", "string", "double", "while", "return", "vector", "set", "map", "std", "cout", "char",
    "unordered_set", "unordered_map", "cin", "cout" 
};

std::unordered_map<std::string, token_type> keyWordMap = {
    { "if", _if }, { "then", _then }, { "else", _else }, { "int", _int }, 
    { "float", _float }, { "bool", _bool }, { "string", _string }, { "double", _double },
    { "while", _while }, { "return", _return }, { "std", _std }, { "cout", _cout }, { "cin", _cin },
    { "char", _char }, { "set", _set }, { "map", _map }, { "unordered_set", _uset }, { "unordered_map", _umap }
};

class Token {
public: 
    token_type type;
    std::string lexeme;

    using ValueType = std::variant<std::monostate, int, double, long long, long double, std::string, char, bool>;
    ValueType value = std::monostate{};

    int symbolTableIndex = -1;
    int literalTableIndex = -1;

    Token() = default;
    Token(token_type t, const std::string l) : type(t), lexeme(l) {}

    template<typename T>
    Token(token_type t, const std::string& l, T val) : type(t), lexeme(l), value(val) {}
};

std::vector<Token> token_sequence;

void token_creator(const std::string& chars, const std::string machine, SymbolTable& symTable, LiteralTable& litTable) {
    if(chars.empty() || machine == "ws") return;
    
    Token token;
    
    if(machine == "id") {
        if(keywords.find(chars) != keywords.end()) token = Token(keyWordMap[chars], "");
        else {
            token = Token(_id, chars);
            token.symbolTableIndex = symTable.addSymbol(chars);
        }
    }
    else if(machine == "integer") {
        token = Token(_integer, chars, std::stoi(chars));
        token.literalTableIndex = litTable.addLiteral(chars, LiteralType::Integer, stoi(chars));
    }
    else if(machine == "double") {
        token = Token(_double, chars, std::stod(chars));
        token.literalTableIndex = litTable.addLiteral(chars, LiteralType::Floating, stod(chars));
    }
    else if(machine == "string") {
        token = Token(_string, chars, chars);
        token.literalTableIndex = litTable.addLiteral(chars, LiteralType::String, chars);
    }
    else if(machine == "character") {
        token = Token(_char, chars, chars[0]);
        token.literalTableIndex = litTable.addLiteral(chars, LiteralType::Character, chars[0]);
    }
    else if(machine == "bool") {
        token = Token(_bool, chars, chars[0]);
        token.literalTableIndex = litTable.addLiteral(chars, LiteralType::Boolean, chars);
    }
    else if(machine == "relop"    )  token = Token(_relop,    chars);
    else if(machine == "aop"      )  token = Token(_aop,      chars);
    else if(machine == "bop"      )  token = Token(_bop,      chars);
    else if(machine == "assign"   )  token = Token(_assign,   chars);
    else if(machine == "end"      )  token = Token(_end,      chars);
    else if(machine == "fas"      )  token = Token(_fas,      chars);
    else if(machine == "fae"      )  token = Token(_fae,      chars);
    else if(machine == "cbs"      )  token = Token(_cbs,      chars);
    else if(machine == "cbe"      )  token = Token(_cbe,      chars);
    else if(machine == "scrop"    )  token = Token(_scrop,    chars);
    else if(machine == "comma"    )  token = Token(_comma,    chars);
    
    token_sequence.push_back(token);
}

std::ostream& operator<<(std::ostream& os, const token_type& t) {
    switch(t) {
        case _integer: os << "integer";     break;
        case _id:      os << "id";          break;
        case _relop:   os << "relop";       break;
        case _aop:     os << "aop";         break;
        case _bop:     os << "bop";         break;
        case _lop:     os << "lop";         break;
        case _assign:  os << "assign";      break;
        case _if:      os << "if";          break;
        case _then:    os << "then";        break;
        case _else:    os << "else";        break;
        case _int:     os << "int";         break;
        case _float:   os << "float";       break;
        case _bool:    os << "bool";        break;
        case _string:  os << "string";      break;
        case _char:    os << "char";        break;
        case _double:  os << "double";      break;
        case _while:   os << "while";       break;
        case _return:  os << "return";      break;
        case _std:     os << "std";         break;
        case _cin:     os << "cin";         break;
        case _cout:    os << "cout";        break;
        case _end:     os << "end";         break;
        case _fas:     os << "fas";         break;
        case _fae:     os << "fae";         break;
        case _cbs:     os << "cbs";         break;
        case _cbe:     os << "cbe";         break;
        case _vector:  os << "vector";      break;
        case _set:     os << "set";         break;
        case _map:     os << "map";         break;
        case _comma:   os << "comma";       break;
        case _scrop:   os << "scrop";       break;
        case _uset:    os << "u_set";       break;
        case _umap:    os << "u_map";       break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << "( " << token.type << ", " << token.lexeme << " )";
    return os;
}