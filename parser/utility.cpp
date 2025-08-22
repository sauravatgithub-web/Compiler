#include <functional>
#include "utility.hpp"
#include "../definitions.hpp"

int recursive_descent_procedure(const Grammar& grammar, std::vector<Token> tokens, int index) {
    std::function<int(Symbol, int)> parsing = [&](Symbol sym, int index) -> int {
        bool parsed = false;

        for(Production p : grammar.productions.at(sym)) {
            int curr_index = index;

            for(Symbol next_sym : p) {
                if(next_sym.name == EPSILON) {
                    parsed = true;
                    continue;
                }

                if(next_sym.nature == Nature::NonTerminal) {
                    curr_index = parsing(next_sym, curr_index);
                    if(curr_index == -1) return -1;
                    else parsed = true;
                }
                else if(next_sym.name == tokens[curr_index].lexeme) {
                    curr_index++;
                }
                else {
                    parsed = false;
                    curr_index = index;
                    break;
                }
            }

            index = curr_index;
        }

        return (parsed == true) ? index : -1;
    };

    return parsing(grammar.startSymbol, 0);
}