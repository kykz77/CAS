#include "algebra.hpp"
#include "parser.hpp"
#include <algorithm>

//Typdef the thing
typedef std::unordered_map<std::string, float> Terms;

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

void get_terms(Node* root, Terms& terms) {

    if (is_const(root->str)) { // For constants
        terms["1"] += root->eval();

    } else if (isalpha(root->str[0])) { // For variables and functions
        terms[root->str] ++;

    } else if (auto* node = dynamic_cast<Add*>(root)) { // If the operator is a +
        get_terms(node->left, terms);
        get_terms(node->right, terms);

    } else if (auto* node = dynamic_cast<Subtract*>(root)) { // If the operator is a - (don't need anymore ... )
        get_terms(node->left, terms);
        Terms neg_terms;
        get_terms(node->right, neg_terms);

        for (auto& [key, value] : neg_terms) {
            terms[key] -= value;
        };

    } else if (auto* node = dynamic_cast<Multiply*>(root)) { // If the operator is a *

        //Get the terms in the left and right subtrees
        Terms left_terms;
        Terms right_terms;
        get_terms(node->left, left_terms);
        get_terms(node->right, right_terms);

        //Declare variables for holding strings, coeff, and power
        std::string key;
        std::string left_var;
        std::string right_var;
        float value;
        int power;

        //Mutliply the terms in the left and right subtrees together, and collect stuff into the appropriate keys if necessary 
        //by considering cases and stuff. 
        for (const auto& [left_key, left_val] : left_terms) {
            
            left_var = get_var(left_key);

            for (const auto& [right_key, right_val] : right_terms) {
                right_var = get_var(right_key);
                value = left_val * right_val;
                
                if (left_key == "1") {
                    key = right_key;
                } else if (right_key == "1") {
                    key = left_key;
                } else if ((left_key == right_key) && (left_key.length() == 1)) {
                    key = left_key + "^2";
                } else if (right_key.rfind(left_key + "^", 0) == 0) {
                    power = std::stoi(right_key.substr(2));
                    key = left_var + "^" + std::to_string(power + 1);
                } else if (left_key.rfind(right_key + "^", 0) == 0) {
                    power = std::stoi(left_key.substr(2));
                    key = left_var + "^" + std::to_string(power + 1);
                } else if ((left_key.rfind(right_var + "^", 0) == 0) && (right_key.rfind(left_var + "^", 0) == 0)) {
                    power = std::stoi(left_key.substr(2)) + std::stoi(right_key.substr(2));
                    key = left_var + "^" + std::to_string(power);
                } else {
                    key = left_key + "*" + right_key;
                };

                terms[key] += value;
            };
        };

    } else if (auto* node = dynamic_cast<Divide*>(root)) { //Operator is a /

        //Declare variables for terms
        Terms left_terms;
        Terms right_terms;
        std::vector<float> left_vec;
        std::vector<float> right_vec;

        //Get the terms on the LHS and RHS
        get_terms(node->left, left_terms);
        get_terms(node->right, right_terms);

        //Convert the terms into vectors
        left_vec = terms_to_vec(left_terms);
        right_vec = terms_to_vec(right_terms);

        //Now divide the left by the right to get the quotient and remainder
        auto [quot_vec, rem_vec] = long_division(left_vec, right_vec);

        //Now convert the quotient 
        Terms quotient = vec_to_terms(quot_vec);
        Terms remain = vec_to_terms(rem_vec);

        //For the quotient add the stuff to the terms
        for (auto& [key, value] : quotient) {
            terms[key] += value;
        };
        
        //For the remainder, add the stuff with the fraction to the terms
        std::string divisor = "/" + node->right->to_string();
        for (auto& [key, value] : remain) {
            terms[key + divisor] += value;
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

Node* expand(Node* root) {
    //TODO: Add a 'terms' attribute to the Node class and then check if terms* is nullptr, if it is get terms otherwise dont 
    //      probably more efficient?????
    Terms terms;
    get_terms(root, terms);

    Node* new_root = nullptr;
    Node* node;
    bool is_letter;
    bool has_pow;
    bool has_div;
    std::string var;

    for (auto& [key, value] : terms) {

        node = new Multiply(new Constant(value) , construct_expr(key));

        if (!new_root) {
            new_root = node;
        } else {
            new_root = new Add(new_root, node);
        };
    };

    return new_root;
};

Node* substitute(Node* root, std::string var, float value) {
    if (root->str == var) {
        return new Constant(value);
    };

    if (auto* node = dynamic_cast<Binary*>(root)) {
        node->left = substitute(node->left, var, value);
        node->right = substitute(node->right, var, value);

    } else if (auto* node = dynamic_cast<Unary*>(root)) {
        node->node = substitute(node->node, var, value);
    };

    return root;
};

bool compare(int a, int b) {
    return (a < b);
};

std::vector<int> get_factors(int num) {
    std::vector<int> factors;
    for (int i = 1; i <= int(std::sqrt(num)); i++) {
        if (num % i == 0) {
            factors.push_back(i);
            if (i != num / i) { // If i is not the square root
                factors.push_back(num / i);
            };
        };
    };
    std::sort(factors.begin(), factors.end(), compare);

    return factors;
};

std::vector<Divide*> rational_root(Terms& terms, int lead_coeff, int const_coeff) {

    //Determine the factors of each coefficient
    std::vector<int> lead_factors = get_factors(lead_coeff);
    std::vector<int> const_factors = get_factors(const_coeff);
    std::vector<Divide*> roots; //The possible rational roots
    
    //Determine the possible rational roots by dividing the numbers by each other
    for (const auto& const_factor : const_factors) {
        for (const auto& lead_factor : lead_factors) {
            roots.push_back(new Divide(new Constant(const_factor) , new Constant(lead_factor))); //Add in a fraction
        };
    };

    return roots;
};

//Use quadratic formula to solve for roots of polynomials, takes in coefficients in vector form with index == degree
std::vector<float> quadratic_formula(std::vector<float> coeff) {

    //Declare variables
    float a, b, c, discrim; //coeffs and discriminant
    std::vector<float> result; //The roots of the quadratic

    //Assign the variables
    a = coeff[2];
    b = coeff[1];
    c = coeff[0];

    //Find the discriminant to determine the 'type' of root, whether there is one root, is it/they rational/irrational, etc
    discrim = std::pow(b, 2) - 4 * a * c;

    //Incase there is no solution, to prevent error (idk if C++ allows complex stuff)
    if (abs(discrim) < 1e-12) { //Do abs(num) < tol due to floating point stuff 
        return {};
    };

    //Take the square root of the disciriminant 
    float root = sqrt(discrim);

    return {(-b - root) / (2 * a) , (-b + root) / (2 * a)};
};

//Get the numerator and denominator and perform long division
std::pair<std::vector<float>, std::vector<float>> long_division(std::vector<float> num, std::vector<float> denom) {
    
    //Get the length of the numerator and denominator
    int len_num = num.size();
    int len_denom = denom.size();
    int offset = len_num - len_denom;
    float lead_coeff = denom[len_denom - 1];
    float factor = 0; //The factor the denom is being multiplied by 
    std::vector<float> quotient(offset + 1, 0); //The result of the division, initilise it with size offset + 1 and with 0s

    // The exit condition is when the length of the numerator is greater than the length of the denominator
    // This is equivalent to degree num >= degree denom for long division to be possible

    while (len_num >= len_denom) {

        //Calculate the factor
        factor = num[len_num - 1] / lead_coeff;

        //Add the factor to the quotient list
        quotient[offset] = factor;

        //For each element in the denom vector, multiply the element by the factor and then subtract it from the 
        //correspoinding element in the numerator
        for (int i = 0; i < len_denom; i++) {
            num[i + offset] -= denom[i] * factor; //Subtract the index from the appropriate thing
        };

        //While the front of the vector is 0 continue to pop the elements from it, and update the length of the vector
        while (num[len_num - 1] == 0) {
            num.pop_back();
            len_num --;
            offset --;
        };
    };

    //Replace with 0 if nothing present i.e. there is no remainder.
    if (len_num == 0) {
        num = {0};
    };

    return std::make_pair(quotient, num); //Return the quoitent and remainder
};

//Convert a terms map into a vector with the appropriate coefficients
std::vector<float> terms_to_vec(Terms terms) {

    //Preallocate a vector for the coeffiients, we can only do degree 10 polys
    //probably a better way to do this lol
    std::vector<float> coeff;
    int max_degree = 10;
    coeff.reserve(max_degree); //max is degree 10 polynomial

    //Iterate through the terms vector and fill in the vector

    //Declare var for the min and max coefficients of the terms
    int lead_coeff = 0; //For max x^n
    int max_pow = 0;
    int const_coeff = 0; //For constant term
    int power;

    //Iterate through the terms to determine the min and max coeffs
    for (const auto& [key, value] : terms) {
        //std::cout << key << " , " << value << std::endl;
        //For getting the coefficient and power
        if (key == "1") {
            const_coeff = int(value);
            power = 0;
        } else {
            power = std::stoi(key.substr(2)); //Get the power of the thing
        };
        
        coeff[power] = value; //Assign the coefficient to the vector

        //For updating the max power
        if (power > max_pow) {
            max_pow = power;
            lead_coeff = value;
        };
    };

    //Now slice the vector to get the correct coefficients, idk how to change end pointer lol
    //Take a slice of the vector depending on the maximum coefficient found
    std::vector<float> cut(coeff.begin(), coeff.begin() + max_pow + 1); //Coefficient counts as 1 so yeah do +1

    return cut;
};

Terms vec_to_terms(std::vector<float> vec) {

    //Initiate terms to hold terms
    Terms terms;
    std::string key;
    float value;

    //Iterate through the vector and put the terms into their appropriate things
    for (int i = 0; i < vec.size(); i++) {
        //Get the correct key
        if (i == 0) {
            key = "1";
        } else {
            key = "x^" + std::to_string(i);
        };

        //Get the value
        value = vec[i];

        //Add KVP to hashmap
        terms[key] += value;
    };

    return terms;
};

Node* factorise(Node* root, std::string var) {

    //Get the terms in the tree
    Terms terms;
    get_terms(root, terms);

    //Pre-allocate a vector to hold the coefficients
    std::vector<float> coeff = terms_to_vec(terms);

    //Declare variables for coeffs
    float lead_coeff = coeff[coeff.size() - 1];
    float const_coeff = coeff[0];

    //Use the rational root function to determine the possible rational roots
    std::vector<Divide*> possible_roots = rational_root(terms, lead_coeff, const_coeff);
    std::vector<Divide*> factors;
    Node* node;

    //Use the factor theorem to determine whether the thing is a root
    for (const auto& value : possible_roots) {
        node = substitute(value, var, value->eval());
        if (node->eval() == 0) {
            factors.push_back(value);
        };
    };

    //Check to see if the degree of the leading coefficient is equal to the number of roots found


    return root;
};