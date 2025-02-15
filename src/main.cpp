#include "parser.hpp"
#include "algebra.hpp"

/*
HOW TO USE:

1. Compile using the line:
g++ -Wno-conversion-null -o outputs/main main.cpp node.cpp parser.cpp algebra.cpp 

2. Run the program using the command:
outputs/main

*/

int main() {
    std::cout << "Original Input:" << std::endl;
    std::string in_1 = "((x^2+3*x^1-4)*(x^1+5)-x^4*5+4*x^1)/(x^1+4)";
    std::cout << in_1 << std::endl;
    std::cout << std::endl;

    std::cout << "Simplified Input:" << std::endl;
    Node* expr_1 = expand(construct_expr(in_1));
    std::cout << expr_1->to_string() << std::endl;
    std::cout << std::endl;

    return 0;
};