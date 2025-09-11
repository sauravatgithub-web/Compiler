#include <iostream>
#include <fstream>
#include <vector>
#include "grammar.hpp"
#include "../definitions.hpp"
#include "../lexical_analysis/analyzer.hpp"

int main() {    
    auto [tokens, symTable, litTable] = getTokens();
    // for(Token token : tokens) std::cout << token << std::endl;

    Grammar grammar;  
    if(grammar.create() == -1) {
        std::exit(EXIT_FAILURE);
    }

    grammar.makeFirsts();
    grammar.makeFollows();

    auto parser = [&](int ptype) {
        switch(ptype) {
            case 1 :  return grammar.recursive_descent_parser(tokens);
            case 2 :  return grammar.LL1_parser(tokens);
            case 3 :  return grammar.LR0_parser(tokens);
            case 4 :  return grammar.SLR1_parser(tokens);
            case 5 :  return grammar.LR1_parser(tokens);
            default : return false;
        }
    };

    if(parser(5)) std::cout << "Parsing completed successfully." << std::endl;
    else {
        std::cerr << "Parsing error" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::cout << "Parsing completed" << std::endl;
    return 0;
}