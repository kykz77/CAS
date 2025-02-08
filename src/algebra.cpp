#include "algebra.hpp"

// Gets the variable from the string
std::string get_var(std::string str) {
    std::string out = "";
    std::string chr;
    int len = str.length();
    for (int i = 0; i < len; i++) {
        chr = str[i];
        if (chr == "^") {
            return out;
        };
        out += chr;
    };
    return out;
};

// Exapands and simplifies the given expression by collecting the like terms
bool is_const(std::string str) {
    if (str.length() == 1) {
        return isdigit(str[0]);
    };

    return (((str[0] == '-') && isdigit(str[1])) || isdigit(str[0]));
};

void simplify_aux(Node* root, Terms& terms) {

    if (is_const(root->str)) { // For constants
        terms["const"] += root->eval();

    } else if (isalpha(root->str[0])) { // For variables and functions
        terms[root->str] ++;

    } else if (auto* node = dynamic_cast<Add*>(root)) { // If the operator is a +
        simplify_aux(node->left, terms);
        simplify_aux(node->right, terms);

    } else if (auto* node = dynamic_cast<Subtract*>(root)) { // If the operator is a - (don't need anymore ... )
        simplify_aux(node->left, terms);
        Terms neg_terms;
        simplify_aux(node->right, neg_terms);

        for (auto& [key, value] : neg_terms) {
            terms[key] -= value;
        };

    } else if (auto* node = dynamic_cast<Multiply*>(root)) { // If the operator is a *
        Terms left_terms;
        Terms right_terms;
        simplify_aux(node->left, left_terms);
        simplify_aux(node->right, right_terms);

        std::string key;
        std::string left_var;
        std::string right_var;

        // std::cout << "Root: " << root->str << std::endl;

        // std::cout << "Left Terms:" << std::endl;
        // for (const auto& [key, value] : left_terms) {
        //     std::cout << key << " , " << value << std::endl;
        // };
        // std::cout << std::endl;

        // std::cout << "Right Terms:" << std::endl;
        // for (const auto& [key, value] : right_terms) {
        //     std::cout << key << " , " << value << std::endl;
        // };
        // std::cout << std::endl;

        float value;

        for (const auto& [left_key, left_val] : left_terms) {
            
            left_var = get_var(left_key);

            for (const auto& [right_key, right_val] : right_terms) {
                right_var = get_var(right_key);
                value = left_val * right_val;

                if (!(left_key == "const" || right_key == "const")) {
                    if ((left_key == right_key) && (left_key.length() == 1)) {
                        key = left_key + "^2";
                    } else if (right_key.rfind(left_key + "^", 0) == 0) {
                        int power = std::stoi(right_key.substr(2));
                        key = left_var + "^" + std::to_string(power + 1);
                    } else if (left_key.rfind(right_key + "^", 0) == 0) {
                        int power = std::stoi(left_key.substr(2));
                        key = left_var + "^" + std::to_string(power + 1);
                    } else if ((left_key.rfind(right_var + "^", 0) == 0) && (right_key.rfind(left_var + "^", 0) == 0)) {
                        int power = std::stoi(left_key.substr(2)) + std::stoi(right_key.substr(2));
                        key = left_var + "^" + std::to_string(power);
                    } else {
                        key = left_key + "*" + right_key;
                    };
                } else if (left_key == "const") {
                    key = right_key;
                } else if (right_key == "const") {
                    key = left_key;
                };

                terms[key] += value;
            };
        };

    } else if (auto* node = dynamic_cast<Power*>(root)) {
        std::string key;

        if (auto* base = dynamic_cast<Variable*>(node->left)) {
            if (auto* val = dynamic_cast<Constant*>(node->right)) {
                key = base->str + "^" + std::to_string(int(val->eval()));
                terms[key] ++;
            };
        } else if (auto* val = dynamic_cast<Constant*>(node->right)) {
            key = val->str + "^" + std::to_string(int(node->left->eval()));
            terms[key] ++;
        };
    };
};

Node* simplify(Node* root) {
    std::unordered_map<std::string, float> terms;
    simplify_aux(root, terms);

    Node* new_root = nullptr;
    Node* node;
    bool is_letter;
    bool has_pow;
    std::string var;

    for (auto& [key, value] : terms) {
        var = get_var(key);
        has_pow = false;
        is_letter = isalpha(var[0]) && (var.length() == 1);

        if (key.length() > 1) {
            if (key[1] == '^') {
                has_pow = true;
            };
        };

        if (key == "const") {
            node = new Constant(value);
        } else if (is_letter && has_pow) {
            int power = std::stoi(key.substr(2));
            node = new Multiply(new Constant(value), new Power(new Variable(NULL), new Constant(power)));
        } else if (is_letter) {
            node = new Multiply(new Constant(value), new Variable(NULL));
        };

        if (!new_root) {
            new_root = node;
        } else {
            new_root = new Add(new_root, node);
        };
    };

    return new_root;
};
