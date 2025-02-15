// =========================================================== START FILE =========================================================== //
#include <iostream>
#include <cmath>
#include "parser.hpp"
#include "algebra.hpp"

// =========================================================== HELPER FUNCTIONS =========================================================== //

std::unordered_map<std::string, std::function<float(float)>> func_map = {
    {"sin", static_cast<float(*)(float)>(std::sin)},
    {"cos", static_cast<float(*)(float)>(std::cos)},
    {"tan", static_cast<float(*)(float)>(std::tan)},
    {"log", static_cast<float(*)(float)>(std::log)},
};

// Function to check if a string is an operator
bool is_operator(const std::string& token) {
    return (token == "+" || token == "-" || token == "*" || token == "/" || token == "^");
}

// Function to check if a string is a function
bool is_function(const std::string& token) {
    return func_map.count(token) > 0;
}

// Function to get operator precedence
int precedence(const std::string& op) {
    if (op == "+" || op == "-") {
        return 1;
    } else if (op == "*" || op == "/") {
        return 2;
    } else if (op == "^") {
        return 3;
    }
    return 0;
}

// =========================================================== INIT =========================================================== //
std::vector<std::string> tokenize(const std::string& expr) {
    std::vector<std::string> tokens;
    std::string chr;
    std::string token;
    int len = expr.length();

    for (int i = 0; i < len; i++) {
        chr = expr[i];
        if (is_operator(chr) || chr == "(" || chr == ")") {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            tokens.push_back(chr);
        } else if (chr != " ") {
            token += chr;
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }
    return tokens;
}

std::vector<std::string> in_to_post(const std::vector<std::string>& tokens) {
    std::vector<std::string> output;
    std::stack<std::string> operators;
    std::string token;
    int len = tokens.size();

    for (int i = 0; i < len; i++) {
        token = tokens[i];
        if (isdigit(token[0]) || isalpha(token[0])) {
            output.push_back(token);
        } else if (is_function(token)) {
            operators.push(token);
        } else if (token == "(") {
            operators.push(token);
        } else if (token == ")") {
            while (!operators.empty() && operators.top() != "(") {
                output.push_back(operators.top());
                operators.pop();
            }
            operators.pop();
            if (!operators.empty() && is_function(operators.top())) {
                output.push_back(operators.top());
                operators.pop();
            }
        } else if (is_operator(token)) {
            while (!operators.empty() && precedence(operators.top()) >= precedence(token) && operators.top() != "(") {
                output.push_back(operators.top());
                operators.pop();
            }
            operators.push(token);
        }
    }

    while (!operators.empty()) {
        output.push_back(operators.top());
        operators.pop();
    }

    return output;
}

// Function to construct the expression tree
Node* construct_tree(const std::vector<std::string>& postfix) {
    std::stack<Node*> stack;
    Node* node;
    Node* left;
    Node* right;

    for (const std::string& token : postfix) {
        if (is_operator(token)) {
            right = stack.top(); stack.pop();
            left = stack.top(); stack.pop();
            if (token == "+") {
                node = new Add(left, right);
            } else if (token == "-") {
                node = new Add(left, right->negate());
            } else if (token == "*") {
                node = new Multiply(left, right);
            } else if (token == "/") {
                node = new Divide(left, right);
            } else if (token == "^") {
                node = new Power(left, right);
            };
            stack.push(node);
        } else if (is_function(token)) {
            node = stack.top(); stack.pop();
            if (token == "sin") {
                node = new Sin(node);
            } else if (token == "cos") {
                node = new Cos(node);
            } else if (token == "tan") {
                node = new Tan(node);
            }
            stack.push(node);
        } else {
            // If the token is a number or a variable, push it to the stack as a Constant or Variable node
            if (isdigit(token[0])) {
                stack.push(new Constant(std::stof(token)));
            } else {
                stack.push(new Variable(NULL)); // For simplicity, variables are treated as having an undefined value
            }
        }
    }

    // The remaining item on the stack is the root of the expression tree
    return stack.top();
}

Node* construct_expr(const std::string& expr) {
    std::vector<std::string> tokens = tokenize(expr);
    std::vector<std::string> postfix = in_to_post(tokens);
    return construct_tree(postfix);
};
