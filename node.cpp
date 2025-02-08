// =========================================================== START FILE =========================================================== //

#include "node.hpp"
#include "algebra.hpp"

// =========================================================== NODE =========================================================== //

// Implementation of Node member functions
Node* Node::negate() {
    return new Multiply(new Constant(-1), this);
};

Node* Node::add(Node* other) {
    return new Add(this, other);
};

Node* Node::subtract(Node* other) {
    return new Add(this, other->negate());
};

Node* Node::multiply(Node* other) {
    return new Multiply(this, other);
};

Node* Node::divide(Node* other) {
    return new Divide(this, other);
};

Node* Node::power(Node* other) {
    return new Power(this, other);
};

// =============== OPERATOR OVERLOADS ================ //

Node::operator std::string() const {
    return to_string();
};

Node* Node::operator+(Node* other) {
    return simplify(add(other));
};

Node* Node::operator-(Node* other) {
    return simplify(subtract(other));
};

Node* Node::operator*(Node* other) {
    return simplify(multiply(other));
};

Node* Node::operator/(Node* other) {
    return simplify(divide(other));
};

Node* Node::operator^(Node* other) {
    return simplify(power(other));
};

// =============== FUNCTIONS ================ //

Node* Node::sin() {
    return new Sin(this);
};

Node* Node::cos() {
    return new Cos(this);
};

Node* Node::tan() {
    return new Tan(this);
};

// =========================================================== VALUE =========================================================== //
// Implementation of Value member functions
Value::Value(float value) {
    this->value = value;
    
};

std::string Value::to_string() const {
    return str;
};

// =========================================================== CONST =========================================================== //
// Implementation of Constant member functions
Constant::Constant(float value) : Value(value) {
    this->str = std::to_string(int(value)); //CHANGE AFTER TESTING DONE TO AVOID RANDOM AHHHHHHHHHH FLOATS
};

float Constant::eval() {
    return value;
};

// =========================================================== VAR =========================================================== //
// Implementation of Variable member functions
Variable::Variable(float value = NULL) : Value(value) {
    this->str = "x"; //CHANGE AFTER TESTING DONE TO AVOID RANDOM AHHHHHHHHHH FLOATS
};

float Variable::eval() {
    return value;
};

// =========================================================== POWER =========================================================== //
// Implementation of Term member functions

// =========================================================== UNARY =========================================================== //
// Implementation of Unary member functions
Unary::Unary(Node* node) {
    this->node = node;
};

std::string Unary::to_string() const {
    return (str + "(" + node->to_string() + ")");
};

// ====================================================== CIRCULAR FUNCTIONS ====================================================== //
// Implementation of Sin member functions
Sin::Sin(Node* node) : Unary(node) {
    this->str = "sin";
};

float Sin::eval() {
    return std::sin(node->eval());
};

// Implementation of Cos member functions
Cos::Cos(Node* node) : Unary(node) {
    this->str = "cos";
};

float Cos::eval() {
    return std::cos(node->eval());
};

// Implementation of Tan member functions
Tan::Tan(Node* node) : Unary(node) {
    this->str = "tan";
};

float Tan::eval() {
    return std::tan(node->eval());
};

// =========================================================== BINARY =========================================================== //
// Implementation of Binary member functions
Binary::Binary(Node* left, Node* right) : left(nullptr) , right(nullptr) {
    this->left = left;
    this->right = right;
};

std::string Binary::to_string() const {
    return ("(" + left->to_string() + " " + str + " " + right->to_string() + ")");
};

// =========================================================== OPERATORS =========================================================== //
// Implementation of Add member functions
Add::Add(Node* left, Node* right) : Binary(left, right) {
    this->str = "+";
};

float Add::eval() {
    return (left->eval() + right->eval());
};

// Implementation of Subtract member functions
Subtract::Subtract(Node* left, Node* right) : Binary(left, right) {
    this->str = "-";
};

float Subtract::eval() {
    return (left->eval() - right->eval());
};

// Implementation of Multiply member functions
Multiply::Multiply(Node* left, Node* right) : Binary(left, right) {
    this->str = "*";
};

float Multiply::eval() {
    return (left->eval() * right->eval());
};

// Implementation of Divide member functions
Divide::Divide(Node* left, Node* right) : Binary(left, right) {
    this->str = "/";
};

float Divide::eval() {
    return (left->eval() / right->eval());
};

// Implementation of Power member functions
Power::Power(Node* left, Node* right) : Binary(left, right) {
    this->str = "^";
};

float Power::eval() {
    return (std::pow(left->eval(), right->eval()));
};

// =========================================================== END FILE =========================================================== //