#include <iostream>
#include <stack>
#include <cctype>
#include "regex_postfix.hpp"

int precedence(char op) {
    switch(op) {
        case '*' : case '+' : case '?' : return 3; 
        case '.' : return 2;
        case '|' : return 1; 
        default: return 0;
    }
}

std::string addSpacing(std::string regex) {
    std::string new_regex;
    size_t i = 0;

    while(i < regex.length()) {
        if(regex[i] == '\\') {
            new_regex.push_back(' ');
            new_regex.push_back('\\');
            new_regex.push_back(regex[i+1]);
            new_regex.push_back(' ');
            i += 2;
        }
        else {
            new_regex.push_back(regex[i]);
            i++;
        }
    }

    if(new_regex.back() == ' ') new_regex.pop_back();
    return new_regex;
}

std::vector<std::string> tokenizeRegex(const std::string& regex) {
    std::vector<std::string> tokens;
    size_t i = 0;

    while(i < regex.length()) {
        if(std::isspace(regex[i])) {
            i++; 
        } 
        else if(regex[i] == '\\') {
            tokens.emplace_back(std::string() + regex[i] + regex[i + 1]);
            i += 2;
        }
        else {
            size_t j = i;
            while(j < regex.length() && (regex[j] != ' ' && regex[j] != '\\')) j++;
            tokens.push_back(regex.substr(i, j - i));
            i = j;
        }
    }

    return tokens;
}

std::vector<std::string> toPostfix(const std::string& regex) {
    std::string new_regex = addSpacing(regex);
    std::vector<std::string> tokens = tokenizeRegex(regex);
 
    std::vector<std::string> output;
    std::stack<std::string> operators;

    for(const std::string& token : tokens) {
        if(token == "\\(") operators.push(token);
        else if(token == "\\)") {
            while(!operators.empty() && operators.top() != "\\(") {
                output.push_back(operators.top());
                operators.pop();
            }
            if(!operators.empty()) operators.pop(); 
        } 
        else if(token[0] == '\\') {
            while(!operators.empty() && operators.top() != "\\(" && precedence(operators.top()[1]) >= precedence(token[1])) {
                output.push_back(operators.top());
                operators.pop();
            }
            operators.push(token);
        }
        else output.push_back(token);
    }

    while(!operators.empty()) {
        output.push_back(operators.top());
        operators.pop();
    }

    return output;
}