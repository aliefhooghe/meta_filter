#ifndef RATIONAL_FRACTION_H_
#define RATIONAL_FRACTION_H_

#include "polynomial.h"

template <typename P1, typename P2>
struct rational_fraction;

template <typename ...E1, typename ...E2>
struct rational_fraction<polynomial<E1...>, polynomial<E2...>> {
    constexpr rational_fraction(const polynomial<E1...>& n, const polynomial<E2...>& d)
    :   numerator{n}, denominator{d} 
    {}

    const polynomial<E1...> numerator;
    const polynomial<E2...> denominator;  
};

template <typename P1, typename P2>
rational_fraction(const P1&, const P2&) -> rational_fraction<P1, P2>;

template <typename P1, typename P2>
decltype(auto) operator<<(std::ostream& stream, const rational_fraction<P1, P2>& r)
{
    return 
        stream << "(" << r.numerator << ") / (" << r.denominator << ")";
}

/*
 * Rational fraction allow to define / operator for polynomials
 */

template <typename ...E1, typename ...E2>
constexpr auto operator/ (const polynomial<E1...>& p1, const polynomial<E2...>& p2)
{
    return rational_fraction{p1, p2};
}

/*
 *  Rational fraction operators 
 */

template <typename P1, typename P2, typename P3, typename P4>
constexpr auto operator +(const rational_fraction<P1, P2>& r1, const rational_fraction<P3, P4>& r2)
{
    return 
        (r1.numerator * r2.denominator + r2.numerator * r1.denominator) /
        (r1.denominator * r2.denominator);
}

template <typename P1, typename P2, typename P3, typename P4>
constexpr auto operator -(const rational_fraction<P1, P2>& r1, const rational_fraction<P3, P4>& r2)
{
    return
        (r1.numerator * r2.denominator - r2.numerator * r1.denominator) /
        (r1.denominator * r2.denominator);
}

template <typename P1, typename P2, typename P3, typename P4>
constexpr auto operator *(const rational_fraction<P1, P2>& r1, const rational_fraction<P3, P4>& r2)
{
    return
        (r1.numerator * r2.numerator) /
        (r1.denominator * r2.denominator);
}

template <typename P1, typename P2, typename P3, typename P4>
constexpr auto operator /(const rational_fraction<P1, P2>& r1, const rational_fraction<P3, P4>& r2)
{
    return rational_fraction{
        (r1.numerator * r2.denominator) /
        (r1.denominator * r2.numerator)
    };
}

/*
 *  Rational fraction/polynomials operators 
 */

template <typename P1, typename P2, typename ...E>
constexpr auto operator +(const rational_fraction<P1, P2>& r, const polynomial<E...>& p)
{
    return 
        (r.numerator + r.denominator * p) / r.denominator;
}

template <typename P1, typename P2, typename ...E>
constexpr auto operator +(const polynomial<E...>& p, const rational_fraction<P1, P2>& r)
{
    return 
        (r.numerator + r.denominator * p) / r.denominator;
}

template <typename P1, typename P2, typename ...E>
constexpr auto operator -(const rational_fraction<P1, P2>& r, const polynomial<E...>& p)
{
    return 
        (r.numerator - r.denominator * p) / r.denominator;
}

template <typename P1, typename P2, typename ...E>
constexpr auto operator -(const polynomial<E...>& p, const rational_fraction<P1, P2>& r)
{
    return 
        (r.denominator * p - r.numerator) / r.denominator;
}

template <typename P1, typename P2, typename ...E>
constexpr auto operator *(const rational_fraction<P1, P2>& r, const polynomial<E...>& p)
{
    return 
        (r.numerator * p) / r.denominator;
}

template <typename P1, typename P2, typename ...E>
constexpr auto operator *(const polynomial<E...>& p, const rational_fraction<P1, P2>& r)
{
    return 
        (r.numerator * p) / r.denominator;
}

template <typename P1, typename P2, typename ...E>
constexpr auto operator /(const rational_fraction<P1, P2>& r, const polynomial<E...>& p)
{
    return 
        r.numerator / (r.denominator * p);
}

template <typename P1, typename P2, typename ...E>
constexpr auto operator /(const polynomial<E...>& p, const rational_fraction<P1, P2>& r)
{
    return 
        (r.denominator * p) / r.numerator;
}

/*
 *  Rational fraction extraction
 */

template <typename E, typename SearchTag>
struct extract_rational_fraction_impl;

template <typename E, typename SearchTag>
constexpr auto extract_rational_fraction(const expression<E>& e, const variable<SearchTag>& var)
{
    return extract_rational_fraction_impl<E, SearchTag>::extract(e, var);
}

template <typename Tag, typename SearchTag>
struct extract_rational_fraction_impl<variable<Tag>, SearchTag> 
{
    static constexpr auto extract(const variable<Tag>& var, const variable<SearchTag>&)
    {
        if constexpr (std::is_same_v<Tag, SearchTag>)
            return polynomial{constant{0}, constant{1}};
        else
            return polynomial{var};
    }
};

template <typename T, typename SearchTag>
struct extract_rational_fraction_impl<constant<T>, SearchTag>
{
    static constexpr auto extract(const constant<T>& cst, const variable<SearchTag>&)
    {
        return polynomial{cst};
    }
};

template <typename Operator, typename E1, typename E2, typename SearchTag>
struct extract_rational_fraction_impl<operation<Operator, E1, E2>, SearchTag>
{
        static constexpr auto extract(const operation<Operator, E1, E2>& op, const variable<SearchTag>& var)
        {
            if constexpr (std::is_same_v<Operator, sum_operation>)
                return extract_rational_fraction(op.operand1, var) +
                    extract_rational_fraction(op.operand2, var);
            else if constexpr (std::is_same_v<Operator, sub_operation>)
                return extract_rational_fraction(op.operand1, var) -
                    extract_rational_fraction(op.operand2, var);
            else if constexpr (std::is_same_v<Operator, product_operation>)
                return extract_rational_fraction(op.operand1, var) *
                    extract_rational_fraction(op.operand2, var);
            else if constexpr (std::is_same_v<Operator, frac_operation>)
                return extract_rational_fraction(op.operand1, var) /
                    extract_rational_fraction(op.operand2, var);
        }
};

#endif