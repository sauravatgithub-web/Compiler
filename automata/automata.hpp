# pragma once

#include <vector>
#include <string>
#include <unordered_set>
#include "fsm_elements.hpp"

class Automata {
public:
    std::vector<State> states;
    std::unordered_set<std::string> alphabets;
    std::vector<Transitions> transitions;
    State initial_state;
};    