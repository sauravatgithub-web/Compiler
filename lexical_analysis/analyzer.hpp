#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <cctype>
#include <fstream>
#include <sstream>
#include "machine.hpp"
#include "token.hpp"
#include "symbol_table.hpp"
#include "literal_table.hpp"
#include "../automata/fsm.hpp"
#include "../automata/dfa.hpp"
#include "../definitions.hpp"

typedef std::tuple<std::vector<Token>, SymbolTable, LiteralTable> Lexical_Ouptut;

Lexical_Ouptut getTokens() {
    std::vector<std::pair<minimized_DFA, std::string>> fsms = create_state_machines();

    std::vector<State_Machine> machines; 
    for(auto fsm : fsms) {
        machines.push_back(State_Machine(fsm));
    }

    std::ifstream infile(CODE_SOURCE_FILE);
    if(!infile.is_open()) {
        std::cerr << "Failed to open code source file" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    SymbolTable symTable;
    LiteralTable litTable;

    std::string line;
    int line_counter = 0;
    std::vector<Token> token_sequence;

    while(std::getline(infile, line)) {
        line_counter++;
        
        int index = 0;
        int length = line.size();
        bool comment_found = false;
        
        while(index < length) {    
            for(auto& machine : machines) machine.reset();
            
            std::string current_lexeme;
            int last_accept_index = -1;
            std::string last_accept_machine;

            int look_ahead = index;

            while(look_ahead < length) {
                char ch = line[look_ahead];
                current_lexeme.push_back(ch);
                if(ch == '/' && line[look_ahead + 1] == '/') {
                    comment_found = true;
                    break;
                }

                bool any_active = false;

                for(auto& machine : machines) {
                    if(!machine.is_null_state()) {
                        machine.update_state(ch);
                        any_active = true;

                        if(machine.is_final_state()) {
                            last_accept_index = look_ahead;
                            last_accept_machine = machine.name;
                        }
                    }
                }

                if(!any_active) break;
                look_ahead++;
            }

            if(comment_found) break;

            if(last_accept_index != -1) {
                std::string token_lexeme = line.substr(index, last_accept_index - index + 1);
                token_creator(token_lexeme, last_accept_machine, symTable, litTable, token_sequence);
                index = last_accept_index + 1;
            } 
            else {
                std::cerr << "Lexical error at line " << line_counter << " index " << index << std::endl;
                std::exit(EXIT_FAILURE);
            }
        }
    }

    return {token_sequence, symTable, litTable};
}