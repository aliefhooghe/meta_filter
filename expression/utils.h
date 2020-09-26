#ifndef UTILS_H_
#define UTILS_H_

#include "expression.h"



template <typename E, typename SearchTag>
struct contains_impl;

template <typename E, typename SearchTag>
constexpr auto contains(
    const expression<E>& e,
    const variable<SearchTag>& var)
{
    return contains_impl<E, SearchTag>::contains(e, var);
}

template <typename Tag, typename SearchTag>
struct contains_impl<variable<Tag>, SearchTag> {
    static constexpr auto contains(
        const variable<Tag>&,
        const variable<SearchTag>&)
    {
        return std::is_same_v<Tag, SearchTag>;
    }
};

template <typename T, typename SearchTag>
struct contains_impl<constant<T>, SearchTag> {
    static constexpr auto contains(
        const constant<T>&,
        const variable<SearchTag>&)
    {
        return false;
    }
};

template <typename Operator, typename E1, typename E2, typename SearchTag>
struct contains_impl<operation<Operator, E1, E2>, SearchTag> {
    static constexpr auto contains(
        const operation<Operator, E1, E2>& e,
        const variable<SearchTag>& var)
    {
        return contains(e.operand1, var) ||
                contains(e.operand2, var);
    }
};

#endif /* UTILS_H_ */