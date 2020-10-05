#ifndef POLYNOMIAL_H_
#define POLYNOMIAL_H_

#include <type_traits>
#include <tuple>
#include <algorithm>

#include "../expression/expression.h"
#include "../expression/print.h"
#include "../utils/integer_range.h"

template <typename ...E>
struct polynomial
{
    constexpr polynomial(const expression<E>& ...e) : coefficients{e...} {}

    static constexpr unsigned int degree() { return sizeof...(E) == 0 ? 0 : sizeof...(E) - 1; }

    const std::tuple<E...> coefficients;
};

template <typename ...E>
decltype(auto) operator<<(std::ostream& stream, const polynomial<E...>& p)
{
    return std::apply(
        [&stream](const auto& ...e) -> std::ostream&
        {
            auto idx = 0;
            return (..., (stream << "(" << e << ")" << "X^" << (idx++) << "  "));
        },
        p.coefficients);
}

/*
 *  Polynomial operators 
 */

template <typename P1, typename P2>
struct add_polynomial_impl;

template <typename ...E1, typename ...E2>
constexpr auto operator+(const polynomial<E1...>& p1, const polynomial<E2...>& p2)
{
    return add_polynomial_impl<polynomial<E1...>, polynomial<E2...>>::add(p1, p2);
}

template <typename ...E1, typename ...E2>
constexpr auto operator-(const polynomial<E1...>& p1, const polynomial<E2...>& p2)
{
    return add_polynomial_impl<polynomial<E1...>, polynomial<E2...>>::sub(p1, p2);
}

template <typename ...E1, typename ...E2>
struct add_polynomial_impl<polynomial<E1...>, polynomial<E2...>>
{
    static constexpr auto add(const polynomial<E1...>& p1, const polynomial<E2...>& p2)
    {
        return add_terms<false>(
            p1, p2, 
            std::make_integer_sequence<unsigned int, std::max(sizeof...(E1), sizeof...(E2))>{});
    }

    static constexpr auto sub(const polynomial<E1...>& p1, const polynomial<E2...>& p2)
    {
        return add_terms<true>(
            p1, p2, 
            std::make_integer_sequence<unsigned int, std::max(sizeof...(E1), sizeof...(E2))>{});
    }
    
    template <bool sub, unsigned int ...I>
    static constexpr auto add_terms(
        const polynomial<E1...>& p1,
        const polynomial<E2...>& p2,
        const std::integer_sequence<unsigned int, I...>)
    {
        return polynomial(add_term<sub, I>(p1, p2)...);
    }

    template <bool sub, unsigned int I>
    static constexpr auto add_term(const polynomial<E1...>& p1, const polynomial<E2...>& p2)
    {
        constexpr auto size1 = sizeof...(E1);
        constexpr auto size2 = sizeof...(E2);
        
        if constexpr (I < size1) {
            if constexpr (I < size2) {
                if constexpr (sub)
                    return std::get<I>(p1.coefficients) - std::get<I>(p2.coefficients);
                else
                    return std::get<I>(p1.coefficients) + std::get<I>(p2.coefficients);
            }
            else {
                return std::get<I>(p1.coefficients);
            }
        }
        else {
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
struct mul_polynomial_impl<polynomial<E1...>, polynomial<E2...>>
{

    static constexpr auto mul(const polynomial<E1...>& p1, const polynomial<E2...>& p2)
    {
        constexpr auto result_degree = 
            polynomial<E1...>::degree() + polynomial<E2...>::degree();
        return mul_impl(
            std::make_integer_sequence<unsigned int , result_degree + 1>{}, p1, p2);
    }

    template <unsigned int ...Degrees>
    static constexpr auto mul_impl(
        const std::integer_sequence<unsigned int, Degrees...>&,
        const polynomial<E1...>& p1,
        const polynomial<E2...>& p2)
    {
        return polynomial{result_monomial<Degrees>(p1, p2)...};
    }

    template <unsigned int Deg>
    static constexpr auto result_monomial(const polynomial<E1...>& p1, const polynomial<E2...>& p2)
    {
        using range = 
            integer_range<
                unsigned int,
                Deg >= polynomial<E2...>::degree() ? Deg - polynomial<E2...>::degree() : 0,
                std::min(Deg, polynomial<E1...>::degree()) + 1>;

        return result_monomial_impl<Deg>(range{}, p1, p2);
    }

    template <unsigned Deg, unsigned int ...I>
    static constexpr auto result_monomial_impl(
        const std::integer_sequence<unsigned int, I...>&,
        const polynomial<E1...>& p1, 
        const polynomial<E2...>& p2)
    {
        return
            ((std::get<I>(p1.coefficients) * std::get<Deg - I>(p2.coefficients)) + ...);
    }

};


#endif /* POLYNOMIAL_H_ */

