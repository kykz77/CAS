// =========================================================== START FILE =========================================================== //
#ifndef PARSER_HPP
#define PARSER_HPP

// =========================================================== IMPORTS =========================================================== //

#include <vector>
#include <stack>
#include <unordered_map>
#include <functional>
#include "node.hpp"

//Initilising the expr tree
std::vector<std::string> tokenize(const std::string& expr); //string -> tokens
std::vector<std::string> in_to_post(const std::vector<std::string>& tokens); // infix to postfix to remove brackets
Node* construct_tree(const std::vector<std::string>& postfix); //Make tree based off stack
Node* construct_expr(const std::string& expr); //Make expression tree based off expression

#endif // PARSER_HPP
// =========================================================== END FILE =========================================================== //
