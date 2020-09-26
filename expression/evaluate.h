#ifndef EVALUATE_H_
#define EVALUATE_H_

#include "expression.h"

/**
 * 
 */

template <typename T>
struct evaluate_impl;

template <typename E>
auto evaluate(const expression<E>& e)
{
    return evaluate_impl<E>::eval(e);
}

template <typename Tag>
struct evaluate_impl<variable<Tag>> {
    static constexpr auto eval(const variable<Tag>&)
    {
        return 1;
    }
};

template <typename T>
struct evaluate_impl<constant<T>> {
    static constexpr auto eval(const constant<T>& cst)
    {
        return cst.value;
    }
};

template <typename E1, typename E2>
struct evaluate_impl<operation<sum_operation, E1, E2>> {
    static constexpr auto eval(const operation<sum_operation, E1, E2>& e)
    {
        return evaluate(e.operand1) + evaluate(e.operand2);
    }
};

template <typename E1, typename E2>
struct evaluate_impl<operation<product_operation, E1, E2>> {
    static constexpr auto eval(const operation<product_operation, E1, E2>& e)
    {
        return evaluate(e.operand1) * evaluate(e.operand2);
    }
};

template <typename E1, typename E2>
struct evaluate_impl<operation<frac_operation, E1, E2>> {
    static constexpr auto eval(const operation<frac_operation, E1, E2>& e)
    {
        return evaluate(e.operand1) / evaluate(e.operand2);
    }
};

template <typename E1, typename E2>
struct evaluate_impl<operation<sub_operation, E1, E2>> {
    static constexpr auto eval(const operation<sub_operation, E1, E2>& e)
    {
        return evaluate(e.operand1) - evaluate(e.operand2);
    }
};

#endif /* EVALUATE_H_ */