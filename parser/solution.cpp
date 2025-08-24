#include <iostream>
#include <fstream>
#include <vector>
#include "grammar.hpp"
#include "utility.hpp"
#include "../definitions.hpp"
#include "../lexical_analysis/analyzer.hpp"

int main() {    
    auto [tokens, symTable, litTable] = getTokens();
    for(Token token : tokens) std::cout << token << std::endl;

    Grammar grammar;  
    if(grammar.create() == -1) {
        std::exit(1);
    }

    grammar.makeFirsts();
    grammar.makeFollows();

    if(recursive_descent_procedure(grammar, tokens, 0) == -1) {
        std::cerr << "Parsing error" << std::endl;
        std::exit(1);
    }

    std::cout << "Parsing completed" << std::endl;
    return 0;
}