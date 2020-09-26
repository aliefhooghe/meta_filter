#ifndef BILINEAR_TRANSFORM_H_
#define BILINEAR_TRANSFORM_H_

#include "../expression/expression.h"
#include "../expression/substitute.h"
#include "print.h"

struct Z_tag;
constexpr variable<Z_tag> Z;

template <>
struct print_impl<variable<Z_tag>>
{
    static constexpr decltype(auto) print(const variable<Z_tag>&, std::ostream& stream)
    {
        return stream << "Z";
    }
};

struct T_tag;
constexpr variable<T_tag> T;

template <>
struct print_impl<variable<T_tag>>
{
    static constexpr decltype(auto) print(const variable<T_tag>&, std::ostream& stream)
    {
        return stream << "T";
    }
};

struct s_tag;
constexpr variable<s_tag> s;

template <>
struct print_impl<variable<s_tag>>
{
    static constexpr decltype(auto) print(const variable<s_tag>&, std::ostream& stream)
    {
        return stream << "s";
    }
};

template <typename E>
constexpr auto ffff(const expression<E>& laplace_transfert_function)
{
    constexpr auto bilinear_transform_approximation = 
        (2 * (Z - 1)) / 
        (T * (Z + 1));
    
    const auto first_order_approx = 
        substitute(laplace_transfert_function, s, bilinear_transform_approximation);

    
}

#endif /* BILINEAR_TRANSFORM_H_ */