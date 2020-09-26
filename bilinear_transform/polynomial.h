#ifndef POLYNOMIAL_H_
#define POLYNOMIAL_H_

#include <type_traits>
#include <tuple>
#include <algorithm>

#include "../expression/expression.h"
#include "../expression/utils.h"
#include "../expression/print.h"

template <typename ...E>
struct polynomial
{
    polynomial(const expression<E>& ...e) : coefficients{e...} {}
    const std::tuple<E...> coefficients;
};

template <typename ...E>
decltype(auto) operator<<(std::ostream& stream, const polynomial<E...>& p)
{
    return std::apply(
        [&stream](const auto& ...e) -> std::ostream&
        {
            auto idx = 0;
            return (..., (stream << "(" << e << ")" << "X^" << (idx++) << " "));
        },
        p.coefficients);
}

//-

template <typename P1, typename P2>
struct add_polynomial_impl;

template <typename ...E1, typename ...E2>
constexpr auto operator+(const polynomial<E1...>& p1, const polynomial<E2...>& p2)
{
    return add_polynomial_impl<polynomial<E1...>, polynomial<E2...>>::add(p1, p2);
}

template <typename ...E1, typename ...E2>
struct add_polynomial_impl<polynomial<E1...>, polynomial<E2...>>
{
    static constexpr auto add(const polynomial<E1...>& p1, const polynomial<E2...>& p2)
    {
        return add_terms(
            p1, p2, 
            std::make_integer_sequence<unsigned int, std::max(sizeof...(E1), sizeof...(E2))>{});
    }
    
    template <unsigned int ...I>
    static constexpr auto add_terms(
        const polynomial<E1...>& p1,
        const polynomial<E2...>& p2,
        const std::integer_sequence<unsigned int, I...>)
    {
        return polynomial(add_term<I>(p1, p2)...);
    }

    template <unsigned int I>
    static constexpr auto add_term(const polynomial<E1...>& p1, const polynomial<E2...>& p2)
    {
        constexpr auto size1 = sizeof...(E1);
        constexpr auto size2 = sizeof...(E2);
        
        if constexpr (I < size1)
        {
            if constexpr (I < size2)
            {
                return std::get<I>(p1.coefficients) + std::get<I>(p2.coefficients);
            }
            else
            {
                return std::get<I>(p1.coefficients);
            }
        }
        else
        {
            static_assert (I < size2);
            return std::get<I>(p2.coefficients);
        }
    }
};

//-

template <typename P1, typename P2>
struct mul_polynomial_impl;

template <typename ...E1, typename ...E2>
constexpr auto operator*(const polynomial<E1...>& p1, const polynomial<E2...>& p2)
{
    return mul_polynomial_impl<polynomial<E1...>, polynomial<E2...>>::mul(p1, p2);
}

template <typename ...E1, typename ...E2>
constexpr auto operator*(const polynomial<E1...>& p1, const polynomial<E2...>& p2)
{
    return mul_polynomial_impl<polynomial<E1...>, polynomial<E2...>>::mul(p1, p2);
}


#endif /* POLYNOMIAL_H_ */
