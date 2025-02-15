# CAS
## Introduction
This project is an open-source computer algebra system (CAS). It will likely never become as advanced as 
something such as wolfram alpha, but I thought it would just be a cool side project to make :) As it is 
in its early stages only polynomials are supported for now, with plans to make it more general in the future. 

Currently, the features include:
- Evaluating expressions
- Expanding expressions

The other features which I am working on include:
- Supporting for implied powers and multiplication
- Make this program into a CLI thing rather than a code editor thing
- Factorising expressions
- Solving algebraic equations symbolically and numerically
- Format the printing better so it looks less cringe

Below are some examples of how the thing will work, enjoy :)

## Examples
### Evaluating Expressions
```cpp
//Import
#include <iostream>
#include <string>
#include "parser.hpp"

//The expression we want to evaluate
std::string expr = "((1/42 * sin(69))*(4*420 + 13*9001)^4)";

//Construct the expression tree
Node* root = construct_expr(expr);

//Evaluate the expression
float result = root->eval();

//Display the result
std::cout << result << std::endl;
```
### Expanding Expressions
```cpp
//Import
#include <iostream>
#include <string>
#include "parser.hpp"
#include "algebra.hpp"

//The expression we want to expand and simplify
std::string expr = "((x^2+3*x^1-4)*(x^1+5)-x^4*5+4*x^1)/(x^1+4)";

//Construct the expression tree
Node* root = construct_expr(expr);

//Expand and simplify the expression
Node* result = expand(root);

//Print out the expanded expression
std::cout << result->to_string() << std::endl;
```
