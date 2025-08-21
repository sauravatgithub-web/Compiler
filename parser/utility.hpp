// void printer(const Symbol* sym, const Production& p, const std::string& input, int index) {
//     std::cout << "\nSentential Form : ";

//     std::cout << sym->name << " --> ";
//     for(Symbol* next_sym : p) std::cout << next_sym->name << " ";
//     std::cout << "\n";

//     std::cout << "Input           : ";
//     for (int i = 0; i < (int)input.size(); i++) {
//         if(i == index) std::cout << "|";   
//         std::cout << input[i];
//     }
//     if(index == (int)input.size()) std::cout << "|"; 
//     std::cout << "\n";
// }

// Procedure Function ------------------------------------------------------
// int procedure(Symbol* sym, std::string sequence, int index) {
//     bool parsed = false;

//     for(Production p : sym->productions) {
//         int curr_index = index;

//         for(Symbol* next_sym : p) {
//             printer(sym, p, sequence, curr_index);
//             if(next_sym->name == EPSILON) {
//                 parsed = true;
//                 continue;
//             }

//             if(non_terminals.count(next_sym->name)) {
//                 curr_index = procedure(next_sym, sequence, curr_index);
//                 if(curr_index == -1) return -1;
//                 else parsed = true;
//             }
//             else if(next_sym->name == sequence[curr_index]) {
//                 curr_index++;
//             }
//             else {
//                 parsed = false;
//                 curr_index = index;
//                 break;
//             }
//         }

//         index = curr_index;
//     }

//     return (parsed == true) ? index : -1;
// }
// -------------------------------------------------------------------------