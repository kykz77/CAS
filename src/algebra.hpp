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
void simplify_aux(Node* root, Terms& terms);

Node* simplify(Node* root);

#endif // ALGEBRA_HPP
