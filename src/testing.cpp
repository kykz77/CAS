#include "algebra.hpp"
#include "node.hpp"
#include "parser.hpp"
#include <iostream>

//g++ -Wno-conversion-null -o outputs/test testing.cpp node.cpp parser.cpp algebra.cpp

int main() {
    //Expressions
    std::cout << "Expression" << std::endl;
    Node* expr_1 = expand(construct_expr("(x^1+1)*(x^1-4)"));

    //Get the terms from first and second expressions
    std::cout << "Terms" << std::endl;
    Terms terms_1;
    get_terms(expr_1, terms_1);

    std::cout << "To Vec" << std::endl;
    std::vector<float> coeff = terms_to_vec(terms_1);
    std::vector<float> result = quadratic_formula(coeff);

    std::cout << "Display result " << std::endl;
    for (const auto& num : result) {
        std::cout << num << " , ";
    };

    return 0;
};