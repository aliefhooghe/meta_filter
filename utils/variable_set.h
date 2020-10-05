#ifndef VARIABLE_SET_H_
#define VARIABLE_SET_H_

#include "../expression/expression.h"
#include "../bilinear_transform/polynomial.h"
#include "../bilinear_transform/rational_fraction.h"
#include "../utils/type_list.h"

template <typename E>
struct variable_set;

template <typename E>
using variable_set_t = typename variable_set<E>::type;

template <typename Tag>
struct variable_set<variable<Tag>>
{
    using type = type_list<Tag>;
};

template <typename T>
struct variable_set<constant<T>>
{
    using type = type_list<>;
};

template <typename T, T Value>
struct variable_set<constexpr_constant<T, Value>>
{
    using type = type_list<>;
};

template <typename Operator, typename E1, typename E2>
struct variable_set<operation<Operator, E1, E2>>
{
    using type = 
        type_list_set_merge_t<
            variable_set_t<E1>,
            variable_set_t<E2>
        >;
};

template <typename ...E>
struct variable_set<polynomial<E...>>
{
    using type = 
        type_list_set_merge_t<
            variable_set_t<E>...
        >;
};

template <typename P1, typename P2>
struct variable_set<rational_fraction<P1, P2>>
{
    using type = 
        type_list_set_merge_t<
            variable_set_t<P1>,
            variable_set_t<P2>
        >;
};

#endif /* VARIABLE_SET_H_ */