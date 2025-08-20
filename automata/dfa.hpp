# pragma once

#include <vector>
#include <map>
#include <set>
#include "nfa.hpp"
#include "automata.hpp"

using StateTransitions = std::map<std::pair<State, std::string>, std::set<State>>;

class minimized_DFA : public Automata {
public:
    std::set<State> final_states;
    std::map<std::pair<State, std::string>, State> dmap;
};

class DFA : public Automata {
public:
    std::vector<State> final_states;
    
    void create_dfa(NFA nfa, StateTransitions& stateTransitions, bool ep);
    minimized_DFA state_reduction();
};