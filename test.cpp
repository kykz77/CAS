#include "node.hpp"
#include <unordered_map>


void collectTerms(Node* node, std::unordered_map<std::string, double>& terms) {
    if (auto* constant = dynamic_cast<Constant*>(node)) {
        terms["const"] += constant->value;
    } else if (auto* variable = dynamic_cast<Variable*>(node)) {
        terms["x"] += 1;
    } else if (auto* add = dynamic_cast<Add*>(node)) {
        collectTerms(add->left, terms);
        collectTerms(add->right, terms);
    } else if (auto* subtract = dynamic_cast<Subtract*>(node)) {
        collectTerms(subtract->left, terms);
        std::unordered_map<std::string, double> negatedTerms;
        collectTerms(subtract->right, negatedTerms);
        for (auto& [key, value] : negatedTerms) {
            terms[key] -= value;
        }
    } else if (auto* multiply = dynamic_cast<Multiply*>(node)) {
        // Recursive handling for x*x*x*... (x^n)
        std::unordered_map<std::string, double> leftTerms;
        std::unordered_map<std::string, double> rightTerms;
        collectTerms(multiply->left, leftTerms);
        collectTerms(multiply->right, rightTerms);

        for (const auto& [leftKey, leftValue] : leftTerms) {
            for (const auto& [rightKey, rightValue] : rightTerms) {
                std::string newKey;
                double newValue = leftValue * rightValue;

                if (leftKey == "x" && rightKey == "x") {
                    newKey = "x^2";
                } else if (leftKey == "x" && rightKey.rfind("x^", 0) == 0) {
                    int power = std::stoi(rightKey.substr(2));
                    newKey = "x^" + std::to_string(power + 1);
                } else if (leftKey.rfind("x^", 0) == 0 && rightKey == "x") {
                    int power = std::stoi(leftKey.substr(2));
                    newKey = "x^" + std::to_string(power + 1);
                } else if (leftKey.rfind("x^", 0) == 0 && rightKey.rfind("x^", 0) == 0) {
                    int leftPower = std::stoi(leftKey.substr(2));
                    int rightPower = std::stoi(rightKey.substr(2));
                    newKey = "x^" + std::to_string(leftPower + rightPower);
                } else if (leftKey == "const") {
                    newKey = rightKey;
                } else if (rightKey == "const") {
                    newKey = leftKey;
                } else {
                    newKey = leftKey + "*" + rightKey;
                }

                terms[newKey] += newValue;
            }
        }
    } else if (auto* power = dynamic_cast<Power*>(node)) {
        // Handle power nodes (e.g., x^2, x^3)
        if (auto* baseVar = dynamic_cast<Variable*>(power->left)) {
            if (auto* expConst = dynamic_cast<Constant*>(power->right)) {
                std::string key = "x^" + std::to_string(static_cast<int>(expConst->value));
                terms[key] += 1;
            }
        }
    }
    // Handle other node types (e.g., Division) if necessary
}


Node* simplifyExpression(Node* root) {
    std::unordered_map<std::string, double> terms;
    collectTerms(root, terms);

    //Bruh
    for (auto& [key, value] : terms) {
        std::cout << key << " , " << value << std::endl;
    };

    Node* simplifiedTree = nullptr;
    for (const auto& [key, value] : terms) {
        Node* termNode;
        if (key == "const") {
            termNode = new Constant(value);
        } else if (key == "x") {
            termNode = new Multiply(new Constant(value), new Variable(NULL));
        } else if (key.rfind("x^", 0) == 0) {
            int power = std::stoi(key.substr(2));
            termNode = new Multiply(new Constant(value), new Power(new Variable(NULL), new Constant(power)));
        }
        if (!simplifiedTree) {
            simplifiedTree = termNode;
        } else {
            simplifiedTree = new Add(simplifiedTree, termNode);
        }
    }
    return simplifiedTree;
}

int main() {
    // Example expression tree: 2*x^4 - x^2 + x*x + 2*x*5 - 5 + (x-1)*(x + 2) - 3
    Node* expr = new Subtract(
        new Subtract(
            new Add(
                new Add(
                    new Subtract(
                        new Add(
                            new Multiply(
                                new Constant(2),
                                new Power(
                                    new Variable(NULL),
                                    new Constant(4)
                                )
                            ),
                            new Multiply(
                                new Variable(NULL),
                                new Variable(NULL)
                            )
                        ),
                        new Power(
                            new Variable(NULL),
                            new Constant(2)
                        )
                    ),
                    new Multiply(
                        new Constant(2),
                        new Multiply(
                            new Variable(NULL),
                            new Constant(5)
                        )
                    )
                ),
                new Multiply(
                    new Subtract(
                        new Variable(NULL),
                        new Constant(1)
                    ),
                    new Add(
                        new Variable(NULL),
                        new Constant(2)
                    )
                )
            ),
            new Constant(5)
        ),
        new Constant(3)
    );

    std::cout << "Original expression tree:" << std::endl;
    std::cout << expr->to_string() << std::endl;

    Node* simplifiedExpr = simplifyExpression(expr);

    std::cout << "Simplified expression tree:" << std::endl;
    std::cout << simplifiedExpr->to_string() << std::endl;

    delete expr;
    delete simplifiedExpr;

    return 0;
}
