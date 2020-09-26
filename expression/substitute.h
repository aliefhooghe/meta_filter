#ifndef SUBSTITUTE_H_
#define SUBSTITUTE_H_

#include "expression.h"

template <typename E1, typename E2, typename SearchTag>
struct substitute_impl;

template <typename E1, typename E2, typename SearchTag>
auto substitute(
    const expression<E1>& e,
    const variable<SearchTag>& var,
    const expression<E2>& substitut)
{
    return substitute_impl<E1, E2, SearchTag>::subst(e, var, substitut);
}

template <typename E1, typename T, typename SearchTag, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
auto substitute(
    const expression<E1>& e,
    const variable<SearchTag>& var,
    T value)
{
    return substitute_impl<E1, constant<T>, SearchTag>::subst(e, var, constant{value});
}

template <typename Tag, typename E, typename SearchTag>
struct substitute_impl<variable<Tag>, E, SearchTag> {
    static constexpr auto subst(
        const variable<Tag>& e,
        const variable<SearchTag>& var,
        const expression<E>& substitut)
    {
        if constexpr (std::is_same_v<Tag, SearchTag>)
            return E{substitut};
        else
            return e;
    }
};

template <typename T, typename E, typename SearchTag>
struct substitute_impl<constant<T>, E, SearchTag> {
    static constexpr auto subst(
        const constant<T>& cst,
        const variable<SearchTag>& var,
        const expression<E>& substitut)
    {
        return cst;
    }
};

template <typename Operator, typename E1, typename E2, typename E3, typename SearchTag>
struct substitute_impl<operation<Operator, E1, E2>, E3, SearchTag> {
    static constexpr auto subst(
        const operation<Operator, E1, E2>& e,
        const variable<SearchTag>& var,
        const expression<E3>& substitut)
    {
        return make_operation<Operator>(
            substitute(e.operand1, var, substitut),
            substitute(e.operand2, var, substitut)
        );
    }
};

#endif /* SUBSTITUTE_H_ */