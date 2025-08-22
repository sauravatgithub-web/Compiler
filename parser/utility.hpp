# pragma once

#include <vector>
#include "symbol.hpp"
#include "grammar.hpp"
#include "../lexical_analysis/token.hpp"

int recursive_descent_procedure(const Grammar& grammar, std::vector<Token> tokens, int index);



// void printer(const Symbol* sym, const Production& p, const std::string& input, int index) {
//     std::cout << "\nSentential Form : ";

//     std::cout << sym->name << " --> ";
//     for(Symbol* next_sym : p) std::cout << next_sym->name << " ";
//     std::cout << "\n";

//     std::cout << "Input           : ";
//     for (int i = 0; i < (int)input.size(); i++) {
//         if(i == index) std::cout << "|";   
//         std::cout << input[i];
//     }
//     if(index == (int)input.size()) std::cout << "|"; 
//     std::cout << "\n";
// }