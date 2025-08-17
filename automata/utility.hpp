# pragma once

#include <string>
#include "nfa.hpp"
#include "dfa.hpp"

StateTransitions createTransitionMap(NFA);
void fill_epsilon_inputs(NFA, StateTransitions&);
void fill_non_epsilon_inputs(NFA, StateTransitions&);


std::pair<minimized_DFA, std::string> create_machine(std::string expression);