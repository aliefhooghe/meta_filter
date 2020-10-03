#ifndef PRINT_H_
#define PRINT_H_

#include "expression.h"
#include <ostream>

/*
 *  Print expression 
 */

template <typename E>
struct print_impl;

template <typename E>
decltype(auto) print(const expression<E>& e, std::ostream& stream)
{
    return print_impl<E>::print(e, stream);
}

template <typename Tag>
struct print_impl<variable<Tag>> {
    static constexpr decltype(auto) print(const variable<Tag>&, std::ostream& stream)
    {
        return stream << "Var";
    }
};

template <typename T>
struct print_impl<constant<T>> {
    static constexpr decltype(auto) print(const constant<T>& cst, std::ostream& stream)
    {
        return stream << cst.value;
    }
};

template <typename T, T Value>
struct print_impl<constexpr_constant<T, Value>> {
    static constexpr decltype(auto) print(const constexpr_constant<T, Value>&, std::ostream& stream)
    {
        return stream << Value;
    }
};

template <typename E1, typename E2>
struct print_impl<operation<sum_operation, E1, E2>> {
    static constexpr decltype(auto) print(const operation<sum_operation, E1, E2>& e, std::ostream& stream)
    {
        stream << "(";
        print_impl<E1>::print(e.operand1, stream);
        stream << " + ";
        print_impl<E2>::print(e.operand2, stream);
        stream << ")";
        return stream;
    }
};

template <typename E1, typename E2>
struct print_impl<operation<product_operation, E1, E2>> {
    static constexpr decltype(auto) print(const operation<product_operation, E1, E2>& e, std::ostream& stream)
    {
        print_impl<E1>::print(e.operand1, stream);
        stream << " * ";
        print_impl<E2>::print(e.operand2, stream);
        return stream;
    }
};

template <typename E1, typename E2>
struct print_impl<operation<frac_operation, E1, E2>> {
    static constexpr decltype(auto) print(const operation<frac_operation, E1, E2>& e, std::ostream& stream)
    {
        stream << "(";
        print_impl<E1>::print(e.operand1, stream);
        stream << " / ";
        print_impl<E2>::print(e.operand2, stream);
        stream << ")";
        return stream;
    }
};

template <typename E1, typename E2>
struct print_impl<operation<sub_operation, E1, E2>> {
    static constexpr decltype(auto) print(const operation<sub_operation, E1, E2>& e, std::ostream& stream)
    {
        stream << "(";
        print_impl<E1>::print(e.operand1, stream);
        stream << " - ";
        print_impl<E2>::print(e.operand2, stream);
        stream << ")";
        return stream;
    }
};

template <typename E>
constexpr decltype(auto) operator<< (std::ostream& stream, const expression<E>& e)
{
    return print(e, stream); 
}


#endif /* PRINT_H_ */