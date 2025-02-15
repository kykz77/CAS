#ifndef ALGEBRA_HPP
#define ALGEBRA_HPP

// =========================================================== IMPORTS =========================================================== //
#include "node.hpp"
#include <unordered_map>

// Typedef the Terms class
typedef std::unordered_map<std::string, float> Terms;

// Gets the variable from the string
std::string get_var(std::string str);

// Simplifies the given expression by collecting the like terms
void get_terms(Node* root, Terms& terms);
std::vector<float> terms_to_vec(Terms terms);
Terms vec_to_terms(std::vector<float> vec);

//Algebric functions
Node* expand(Node* root); //Expands the function
Node* factorise(Node* root, std::string var); //Factorises the function
Node* substitute(Node* root, std::string var, float value); //substitute value for variable
std::vector<float> quadratic_formula(std::vector<float> coeff);
std::pair<std::vector<float>, std::vector<float>> long_division(std::vector<float> num, std::vector<float> denom); //poly long divis
bool compare(int a, int b); 
std::vector<int> get_factors(int num);
std::vector<Divide*> rational_root(Terms& terms, int lead_coeff, int const_coeff);



#endif // ALGEBRA_HPP
