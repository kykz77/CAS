#include "algebra.hpp"
#include "node.hpp"
#include "parser.hpp"
#include <iostream>

//g++ -Wno-conversion-null -o outputs/test testing.cpp node.cpp parser.cpp algebra.cpp

int main() {
    // //Expressions
    std::cout << "Expression" << std::endl;
    Node* expr_1 = expand(construct_expr("x^4-4*x^3-8*x^2+12*x^1+15"));

    std::cout << "Expanded" << std::endl;
    std::cout << expr_1->to_string() << std::endl;

    Node* result = factorise(expr_1, "x");
    std::cout << result->to_string() << std::endl;
    
    // std::vector<float> num = {-20,5,4,-1};
    // std::vector<float> denom = {-1,4};
    // auto [quotient, remain] = long_division(num, denom);
    // for (const auto& num : quotient) {
    //     std::cout << num << " , ";
    // };
    return 0;
};