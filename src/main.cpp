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
    std::cout << "Original Inputs:" << std::endl;
    std::string in_1 = "(x+1)*(x^2+3*x-5)*(x+4)";
    std::string in_2 = "(x*x-4)*(x+1-6)";
    std::cout << in_1 << std::endl;
    std::cout << in_2 <<std::endl;
    std::cout << std::endl;

    std::cout << "Simplified Inputs:" << std::endl;
    Node* expr_1 = construct_expr(in_1);
    std::cout << expr_1->to_string() << std::endl;
    Node* expr_2 = construct_expr(in_2);
    std::cout << expr_2->to_string() << std::endl;
    std::cout << std::endl;

    Node* result = *expr_1 + expr_2;

    std::cout << "Result: (in_1 + in_2)" << std::endl;
    std::cout << result->to_string() << std::endl;

    return 0;
};