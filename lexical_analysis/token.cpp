#include <utility>
#include <unordered_map>
#include <unordered_set>
#include "token.hpp"

std::unordered_set<std::string> keywords = { 
    "if", "then", "else", "int", "float", "bool", "string", "double", "while", "return", "vector", "set", "map", "std", "cout", "char",
    "unordered_set", "unordered_map", "cin", "cout", "main", "long" , "endl"
};

std::unordered_map<std::string, token_type> keyWordMap = {
    { "if", token_type::_if }, { "then", token_type::_then }, { "else", token_type::_else }, { "int", token_type::_int }, 
    { "float", token_type::_float }, { "bool", token_type::_bool }, { "string", token_type::_string }, { "double", token_type::_double },
    { "while", token_type::_while }, { "return", token_type::_return }, { "std", token_type::_std }, { "cout", token_type::_cout }, { "cin", token_type::_cin },
    { "char", token_type::_char }, { "set", token_type::_set }, { "map", token_type::_map }, { "unordered_set", token_type::_uset }, 
    { "unordered_map", token_type::_umap }, { "main", token_type::_main }, { "long", token_type::_long }, { "endl", token_type::_endl }
};

void token_creator(const std::string& chars, const std::string machine, SymbolTable& symTable, LiteralTable& litTable, std::vector<Token>& token_sequence) {
    if(chars.empty() || machine == "ws") return;
    
    Token token;
    
    if(machine == "id") {
        if(keywords.find(chars) != keywords.end()) token = Token(keyWordMap[chars], "");
        else {
            token = Token(token_type::_id, chars);
            token.symbolTableIndex = symTable.addSymbol(chars);
        }
    }
    else if(machine == "int_lit") {
        token = Token(token_type::_int_lit, chars, std::stoi(chars));
        token.literalTableIndex = litTable.addLiteral(chars, LiteralType::Integer, stoi(chars));
    }
    else if(machine == "doub_lit") {
        token = Token(token_type::_doub_lit, chars, std::stod(chars));
        token.literalTableIndex = litTable.addLiteral(chars, LiteralType::Floating, stod(chars));
    }
    else if(machine == "str_lit") {
        token = Token(token_type::_str_lit, chars, chars);
        token.literalTableIndex = litTable.addLiteral(chars, LiteralType::String, chars);
    }
    else if(machine == "char_lit") {
        token = Token(token_type::_char_lit, chars, chars[0]);
        token.literalTableIndex = litTable.addLiteral(chars, LiteralType::Character, chars[0]);
    }
    else if(machine == "bool") {
        token = Token(token_type::_bool, chars, chars[0]);
        token.literalTableIndex = litTable.addLiteral(chars, LiteralType::Boolean, chars);
    }
    else if(machine == "relop"    )  token = Token(token_type::_relop,    chars);
    else if(machine == "aop"      )  token = Token(token_type::_aop,      chars);
    else if(machine == "bop"      )  token = Token(token_type::_bop,      chars);
    else if(machine == "assignop" )  token = Token(token_type::_assignop, chars);
    else if(machine == "end"      )  token = Token(token_type::_end,      chars);
    else if(machine == "fas"      )  token = Token(token_type::_fas,      chars);
    else if(machine == "fae"      )  token = Token(token_type::_fae,      chars);
    else if(machine == "cbs"      )  token = Token(token_type::_cbs,      chars);
    else if(machine == "cbe"      )  token = Token(token_type::_cbe,      chars);
    else if(machine == "scrop"    )  token = Token(token_type::_scrop,    chars);
    else if(machine == "comma"    )  token = Token(token_type::_comma,    chars);
    else if(machine == "larrow"   )  token = Token(token_type::_larrow,   chars);
    else if(machine == "rarrow"   )  token = Token(token_type::_rarrow,   chars);
    else if(machine == "dlarrow"  )  token = Token(token_type::_dlarrow,  chars);
    else if(machine == "drarrow"  )  token = Token(token_type::_drarrow,  chars);
    
    token_sequence.push_back(token);
}

std::ostream& operator<<(std::ostream& os, const token_type& t) {
    switch(t) {
        case token_type::_integer:     os << "integer";     break;
        case token_type::_id:          os << "id";          break;
        case token_type::_relop:       os << "relop";       break;
        case token_type::_aop:         os << "aop";         break;
        case token_type::_bop:         os << "bop";         break;
        case token_type::_lop:         os << "lop";         break;
        case token_type::_assignop:    os << "assignop";    break;
        case token_type::_if:          os << "if";          break;
        case token_type::_then:        os << "then";        break;
        case token_type::_else:        os << "else";        break;
        case token_type::_int:         os << "int";         break;
        case token_type::_float:       os << "float";       break;
        case token_type::_bool:        os << "bool";        break;
        case token_type::_string:      os << "string";      break;
        case token_type::_char:        os << "char";        break;
        case token_type::_double:      os << "double";      break;
        case token_type::_while:       os << "while";       break;
        case token_type::_return:      os << "return";      break;
        case token_type::_std:         os << "std";         break;
        case token_type::_cin:         os << "cin";         break;
        case token_type::_cout:        os << "cout";        break;
        case token_type::_end:         os << "end";         break;
        case token_type::_fas:         os << "fas";         break;
        case token_type::_fae:         os << "fae";         break;
        case token_type::_cbs:         os << "cbs";         break;
        case token_type::_cbe:         os << "cbe";         break;
        case token_type::_vector:      os << "vector";      break;
        case token_type::_set:         os << "set";         break;
        case token_type::_map:         os << "map";         break;
        case token_type::_comma:       os << "comma";       break;
        case token_type::_scrop:       os << "scrop";       break;
        case token_type::_uset:        os << "u_set";       break;
        case token_type::_umap:        os << "u_map";       break;
        case token_type::_int_lit:     os << "int_lit";     break;
        case token_type::_doub_lit:    os << "doub_lit";    break;
        case token_type::_char_lit:    os << "char_lit";    break;
        case token_type::_str_lit:     os << "str_lit";     break;
        case token_type::_main:        os << "main";        break;
        case token_type::_long:        os << "long";        break;
        case token_type::_dollar:      os << "$";           break;
        case token_type::_larrow:      os << "larrow";      break;
        case token_type::_rarrow:      os << "rarrow";      break;
        case token_type::_dlarrow:     os << "dlarrow";     break;
        case token_type::_drarrow:     os << "drarrow";     break;
        case token_type::_endl:        os << "endl";        break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << "( " << token.type << ", " << token.lexeme << " )";
    return os;
}