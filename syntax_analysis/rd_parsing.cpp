#include "grammar.hpp"

bool Grammar::recursive_descent_parser(const std::vector<Token>& tokens) {
    std::vector<int> results = recursive_descent_main_procedure(startSymbol, tokens, 0);
    for(int idx : results) {
        if(idx == (int)tokens.size()) return true;
    }
    return false;
}

std::vector<int> Grammar::recursive_descent_main_procedure(const Symbol& sym, const std::vector<Token>& tokens, int index) {
    std::vector<int> results;
    for(const Production& p : productions[sym]) {
        std::vector<int> subresults = recursive_descent_procedure(p, tokens, index);
        results.insert(results.end(), subresults.begin(), subresults.end());
    }
    return results;
}

std::vector<int> Grammar::recursive_descent_procedure(const Production& prod, const std::vector<Token> tokens, int index) {
    std::vector<int> current = {index};

    for(Symbol next_sym : prod) {
        std::vector<int> next_indices;

        for(int idx : current) {
            if(next_sym.isEpsilon()) {
                next_indices.push_back(idx);
            }
            else if(next_sym.nature == Nature::NonTerminal) {
                std::vector<int> subresults = recursive_descent_main_procedure(next_sym, tokens, idx);
                next_indices.insert(next_indices.end(), subresults.begin(), subresults.end());
            }
            else {
                if(idx < (int)tokens.size() && tokens[idx].lexeme == next_sym.name) {
                    next_indices.push_back(idx + 1);
                }
            }
        }

        if(next_indices.empty()) return {};
        current = std::move(next_indices);
    }

    return current;
}