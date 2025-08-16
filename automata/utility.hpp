# pragma once

#include <string>
#include <set>
#include <map>
#include "regex_postfix.hpp"
#include "fsm_elements.hpp"
#include "nfa.hpp"
#include "dfa.hpp"

StateTransitions createTransitionMap(NFA);
void fill_epsilon_inputs(NFA, StateTransitions&);
void fill_non_epsilon_inputs(NFA, StateTransitions&);


std::pair<minimized_DFA, std::string> create_machine(std::string expression) {
    int length = expression.size();

    std::string machine_name;
    int index = 0;

    while(index < length) {
        if(expression[index] == ':') break;
        if(expression[index] != ' ') machine_name.push_back(expression[index]);
        index++;
    }

    expression = expression.substr(index + 2);

    std::vector<std::string> postfix = toPostfix(expression);

    NFA nfa = NFA();
    nfa.create_nfa(postfix);
    
    StateTransitions stateTransitions = createTransitionMap(nfa);
    
    DFA dfa = DFA();
    dfa.create_dfa(nfa, stateTransitions, stateTransitions[{nfa.initial_state, EPSILON}].count(nfa.final_state));
    
    minimized_DFA mini_dfa = dfa.state_reduction();
    return {mini_dfa, machine_name};
}

StateTransitions createTransitionMap(NFA nfa) {
    StateTransitions stateTransitions;

    fill_epsilon_inputs(nfa, stateTransitions);
    fill_non_epsilon_inputs(nfa, stateTransitions);

    return stateTransitions;
}

void fill_epsilon_inputs(NFA nfa, StateTransitions& stateTransitions) {
    for(Transitions t : nfa.transitions) {
        stateTransitions[{t.initial_state, t.input}].insert(t.final_state);
    }
    for(State s : nfa.states) stateTransitions[{s, EPSILON}].insert(s);
    
    for(auto& transits : stateTransitions) {
        if(transits.first.second == EPSILON) {
            for(State state : transits.second) {
                for(State nextStates : stateTransitions[{state, EPSILON}]) {
                    transits.second.insert(nextStates);
                }
            }
        }
    }
}

void fill_non_epsilon_inputs(NFA nfa, StateTransitions& stateTransitions) {
    for(std::string input : nfa.alphabets) {
        for(State s : nfa.states) {            
            for(State state : stateTransitions[{s, EPSILON}]) {
                for(State nextState : stateTransitions[{state, input}]) {
                    for(State actualState : stateTransitions[{nextState, EPSILON}]) {
                        stateTransitions[{s, input}].insert(actualState);
                    }
                }
            }
        }
    }
}