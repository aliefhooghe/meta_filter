#include <iostream>

#include "expression/expression.h"
#include "expression/evaluate.h"
#include "expression/substitute.h"
#include "expression/print.h"

#include "bilinear_transform/polynomial.h"

int main()
{
    variable<int> a;
    variable<float> b;
    variable<char> c;

    auto p1 = polynomial{a, b, c};
    auto p2 = polynomial{a+1, a};

    auto p3 = p1 + p2;

    std::cout << p1 << std::endl;;
    std::cout << p2 << std::endl;;
    std::cout << p3 << std::endl;;
 
    return 0;
}

