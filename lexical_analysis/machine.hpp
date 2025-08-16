#pragma once

#include <string>
#include <cctype>
#include "../definitions.hpp"
#include "../automata/dfa.hpp"
#include "../automata/fsm_elements.hpp"

class State_Machine {
private:
    minimized_DFA dfa;
    
public:
    std::string name;
    State current_state;
    
    State_Machine() {}
    State_Machine(const std::pair<minimized_DFA, std::string>& curr_dfa) {
        dfa = curr_dfa.first;
        current_state = dfa.initial_state;
        name = curr_dfa.second;
    }   

    void update_state(char ch) {
        std::string input;
        if(dfa.alphabets.count(std::string(1, ch))) input = std::string(1, ch);
        else if(dfa.alphabets.count("char")) input = "char";
        else if(std::isalpha(ch) && dfa.alphabets.count("letter")) input = "letter";
        else if(std::isdigit(ch) && dfa.alphabets.count("digit")) input = "digit";
        else if(std::isspace(ch) && dfa.alphabets.count("white_space")) input = "white_space";
        else input = "other";
        
        if(dfa.dmap.count({current_state, input})) {
            current_state =  dfa.dmap[{current_state, input}];            
        }
        else current_state = NULL_STATE;
    }

    void reset() { current_state = dfa.initial_state; }
    bool is_null_state() { return current_state == NULL_STATE; }
    bool is_final_state() { return dfa.final_states.count(current_state); }
};