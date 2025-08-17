# pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include "dfa.hpp"
#include "utility.hpp"
#include "../definitions.hpp"

std::vector<std::pair<minimized_DFA, std::string>> create_state_machines() {
    std::ifstream infile(REGEX_SOURCE_FILE);
    if(!infile.is_open()) {
        std::cerr << "Failed to open regex source file." << std::endl;
        return {};
    }

    std::vector<std::pair<minimized_DFA, std::string>> state_machines;

    std::string expression;
    while(std::getline(infile, expression)) {
        while(std::isspace(expression.back())) {
            expression.pop_back();
        }
        std::pair<minimized_DFA, std::string> FSM = create_machine(expression);
        state_machines.push_back(FSM);
    }

    return state_machines;
}