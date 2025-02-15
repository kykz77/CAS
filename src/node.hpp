// =========================================================== START FILE =========================================================== //
#ifndef NODE_HPP
#define NODE_HPP

// =========================================================== IMPORTS =========================================================== //
#include <iostream>
#include <string>
#include <cmath>
#include <stack>
#include <vector>

// ====================================================== CLASS DECLARATION ====================================================== //
class Node;
class Unary;
class Binary;
class Value;
class Constant;
class Variable;
class Term;
class Sin;
class Cos;
class Tan;
class Add;
class Subtract;
class Multiply;
class Divide;
class Power;

// =========================================================== NODE =========================================================== //
class Node {
    public:
        // Call to get value from class
        std::string str;
        virtual std::string to_string() const = 0;
        virtual float eval() = 0; // Abstract class method
        virtual ~Node() = default;

        // Performing basic operations on the node
        Node* negate();
        Node* add(Node* other);
        Node* subtract(Node* other);
        Node* multiply(Node* other);
        Node* divide(Node* other);
        Node* power(Node* other);

        //Calculus operators TODO
        // Node* differentiate(); //returns derivative of function 
        // Node* integrate(); //returns integral of function (IF POSSIBLE)

        // Operator overload for converting into string
        operator std::string() const;
        Node* operator+(Node* other);
        Node* operator-(Node* other);
        Node* operator*(Node* other);
        Node* operator/(Node* other);
        Node* operator^(Node* other);

        // Add the function classes which can be done on the node, just use the basics for now
        Node* sin();
        Node* cos();
        Node* tan();
};

// =========================================================== VALUE =========================================================== //
class Value : public Node {
    public:
        float value;

    public:
        Value() = default;
        Value(float value);

        std::string to_string() const override;
};

// =========================================================== CONST =========================================================== //
class Constant : public Value {
    public:
        Constant(float value);

        float eval() override;
};

// =========================================================== VAR =========================================================== //
class Variable : public Value {
    public:
        Variable(float value);

        float eval() override;
};

// =========================================================== UNARY =========================================================== //
class Unary : public Node {
    public:
        Node* node;

    public:
        Unary(Node* node);

        std::string to_string() const override;
};

// ====================================================== CIRCULAR FUNCTIONS ====================================================== //
class Sin : public Unary {
    public:
        Sin(Node* node);

        float eval() override;
};

class Cos : public Unary {
    public:
        Cos(Node* node);

        float eval() override;
};

class Tan : public Unary {
    public:
        Tan(Node* node);

        float eval() override;
};

// =========================================================== BINARY =========================================================== //
class Binary : public Node {
    public:
        Node* left;
        Node* right;

    public:
        Binary(Node* left = nullptr, Node* right = nullptr);

        std::string to_string() const override;
};

// =========================================================== OPERATORS =========================================================== //
class Add : public Binary {
    public:
        Add(Node* left, Node* right);

        float eval() override;
};

class Subtract : public Binary {
    public:
        Subtract(Node* left, Node* right);

        float eval() override;
};

class Multiply : public Binary {
    public:
        Multiply(Node* left, Node* right);

        float eval() override;
};

class Divide : public Binary {
    public:
        Divide(Node* left, Node* right); //Assume left == numerator, right == denominator

        float eval() override;
};

class Power : public Binary {
    public:
        Power(Node* left, Node* right); //Assume left == base and right == exponent

        float eval() override;
};

#endif // NODE_HPP

// =========================================================== END FILE =========================================================== //