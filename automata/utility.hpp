# pragma once

#include <string>
#include "nfa.hpp"
#include "dfa.hpp"

StateTransitions createTransitionMap(NFA nfa);
void fill_epsilon_inputs(NFA nfa, StateTransitions& stateTransitions);
void fill_non_epsilon_inputs(NFA nfa, StateTransitions& stateTransitions);

std::pair<minimized_DFA, std::string> create_machine(std::string expression);