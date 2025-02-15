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

    //Expressions
    std::string expr = "x^4-4*x^3-8*x^2+12*x^1+15";
    std::cout << "Expression" << std::endl;
    std::cout << expr << std::endl;
    std::cout << std::endl;

    Node* expr_1 = expand(construct_expr(expr));

    std::cout << "Expanded" << std::endl;
    std::cout << expr_1->to_string() << std::endl;
    std::cout << std::endl;

    std::cout << "Factorise" << std::endl;
    Node* result = factorise(expr_1, "x");
    std::cout << result->to_string() << std::endl;
    std::cout << std::endl;

    std::cout << "Zeros" << std::endl;
    std::vector<float> roots = zeros(expr_1, "x");
    for (const auto& root : roots) {
        std::cout << root << " , ";
    };

    std::cout << std::endl;

    return 0;
};