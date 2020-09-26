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

template <typename E, typename SearchTag>
auto as_rational_fraction(const expression<E>& e, const variable<SearchTag>&)
{

}


#endif