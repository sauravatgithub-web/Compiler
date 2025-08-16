# pragma once

#include <iostream>

class State {
public:
    int val;
    
    State() { val = 0; }
    State(int v) { val = v; }

    bool operator==(const State& other) const {
        return val == other.val;
    }    

    bool operator!=(const State& other) const {
        return val != other.val;
    }    

    bool operator<(const State& other) const {
        return val < other.val;
    }
};

class Transitions {
public:
    std::string input;
    State initial_state;
    State final_state;

    Transitions(std::string input_string, State s1, State s2) {
        input = input_string, initial_state = s1, final_state = s2;
    }
};