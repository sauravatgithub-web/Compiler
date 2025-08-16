# pragma once

#include <vector>
#include <string>
#include "fsm_elements.hpp"
#include "automata.hpp"

class NFA : public Automata {
public:
    State final_state;

    void create_nfa(std::vector<std::string> regex);
};