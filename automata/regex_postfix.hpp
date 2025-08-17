# pragma once

#include <string>
#include <vector>

std::string addSpacing(std::string regex);
std::vector<std::string> tokenizeRegex(const std::string& regex);
std::vector<std::string> toPostfix(const std::string& regex);