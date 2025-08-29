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

    // if(grammar.recursive_descent_parser(tokens)) std::cout << "Parsing completed successfully." << std::endl;
    // else {
    //     std::cerr << "Parsing error" << std::endl;
    //     std::exit(EXIT_FAILURE);
    // }

    if(grammar.LL1_parser(tokens)) std::cout << "Parsing completed successfully." << std::endl;
    else {
        std::cerr << "Parsing error" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::cout << "Parsing completed" << std::endl;
    return 0;
}