
#ifndef EXPRESION_H_
#define EXPRESION_H_

#include <type_traits>

/**
 * 
 */
template <typename E>
struct expression
{
    constexpr operator const E&() const
    {
        return *static_cast<const E*>(this);
    }
};

/**
 * 
 */
template <typename Tag>
struct variable : expression<variable<Tag>> {};

/**
 * 
 */
template <typename T>
struct constant : expression<constant<T>>
{
    constexpr constant(T val) : value{val} {}
    constexpr constant(const constant& c) : value{c.value} {}
    const T value;
};

/**
 * 
 */
template <typename T, T Value>
struct constexpr_constant : expression<constexpr_constant<T, Value>>, std::integral_constant<T, Value>{};

/**
 * 
 */
template <typename Operator, typename E1, typename E2>
struct operation : expression<operation<Operator, E1, E2>>
{
    constexpr operation(const E1& e1, const E2& e2) :  operand1{e1}, operand2{e2} {}
    const E1 operand1;
    const E2 operand2;
};

template <typename Operator, typename E1, typename E2>
auto make_operation(const expression<E1>& e1, const expression<E2>& e2)
{
    return operation<Operator, E1, E2>{e1, e2};
}

struct sum_operation;
struct product_operation;
struct frac_operation;
struct sub_operation;

template <typename E1, typename E2>
constexpr auto operator+(const expression<E1>& e1, const expression<E2>& e2)
{
    return operation<sum_operation, E1, E2>{e1, e2};
}

template <typename E, typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto operator+(const expression<E>& e, T cst)
{
    return operation<sum_operation, E, constant<T>>{e, cst};
}

template <typename T, typename E, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto operator+(T cst, const expression<E>& e)
{
   return e + cst;
}

template <typename E, typename T, T Value>
constexpr auto operator+(const expression<E>& e, const constexpr_constant<T, Value>& cst)
{
    if constexpr (Value == 0)
        return E{e};
    else
        return operation<sum_operation, E, constexpr_constant<T, Value>>{e, cst};
}

template <typename E, typename T, T Value>
constexpr auto operator+(const constexpr_constant<T, Value>& cst, const expression<E>& e)
{
    return e + cst;
}

template <typename T1, T1 Value1, typename T2, T2 Value2>
constexpr auto operator+(const constexpr_constant<T1, Value1>& cst1, const constexpr_constant<T2, Value2>& cst2)
{
    return constexpr_constant<
        decltype(std::declval<T1>() + std::declval<T2>()), Value1 + Value2>{};
}

//-

template <typename E1, typename E2>
constexpr auto operator*(const expression<E1>& e1, const expression<E2>& e2)
{
    return operation<product_operation, E1, E2>{e1, e2};
}

template <typename E, typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto operator*(const expression<E>& e, T cst)
{
    return operation<product_operation, E, constant<T>>{e, cst};
}

template <typename T, typename E, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto operator*(T cst, const expression<E>& e)
{
    return operation<product_operation, constant<T>, E>{cst, e};
}

template <typename E, typename T, T Value>
constexpr auto operator*(const expression<E>& e, const constexpr_constant<T, Value>& cst)
{
    if constexpr (Value == 0)
        return constexpr_constant<T, 0>{};
    else if constexpr (Value == 1)
        return E{e};
    else
        return operation<product_operation, E, constexpr_constant<T, Value>>{e, cst};
}

template <typename E, typename T, T Value>
constexpr auto operator*(const constexpr_constant<T, Value>& cst, const expression<E>& e)
{
    return e * cst;
}

template <typename T1, T1 Value1, typename T2, T2 Value2>
constexpr auto operator*(const constexpr_constant<T1, Value1>& cst1, const constexpr_constant<T2, Value2>& cst2)
{
    return constexpr_constant<
        decltype(std::declval<T1>() * std::declval<T2>()), Value1 * Value2>{};
}

//-

template <typename E1, typename E2>
constexpr auto operator/(const expression<E1>& e1, const expression<E2>& e2)
{
    return operation<frac_operation, E1, E2>{e1, e2};
}

template <typename E, typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto operator/(const expression<E>& e, T cst)
{
    return operation<frac_operation, E, constant<T>>{e, cst};
}

template <typename T, typename E, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto operator/(T cst, const expression<E>& e)
{
    return operation<frac_operation, constant<T>, E>{cst, e};
}

template <typename E, typename T, T Value>
constexpr auto operator/(const expression<E>& e, const constexpr_constant<T, Value>& cst)
{
    static_assert(Value != 0);

    if constexpr (Value == 1)
        return E{e};
    else
        return operation<frac_operation, E, constexpr_constant<T, Value>>{e, cst};
}

template <typename E, typename T, T Value>
constexpr auto operator/(const constexpr_constant<T, Value>& cst, const expression<E>& e)
{
    if constexpr (Value == 0)
        return cst;
    else
        return operation<frac_operation, E, constexpr_constant<T, Value>>{cst, e};
}

template <typename T1, T1 Value1, typename T2, T2 Value2>
constexpr auto operator/(const constexpr_constant<T1, Value1>& cst1, const constexpr_constant<T2, Value2>& cst2)
{
    return constexpr_constant<
        decltype(std::declval<T1>() / std::declval<T2>()), Value1 / Value2>{};
}

//-

template <typename E1, typename E2>
constexpr auto operator-(const expression<E1>& e1, const expression<E2>& e2)
{
    return operation<sub_operation, E1, E2>{e1, e2};
}

template <typename E, typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto operator-(const expression<E>& e, T cst)
{
    return operation<sub_operation, E, constant<T>>{e, cst};
}

template <typename T, typename E, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
constexpr auto operator-(T cst, const expression<E>& e)
{
    return operation<sub_operation, constant<T>, E>{cst, e};
}

template <typename E, typename T, T Value>
constexpr auto operator-(const expression<E>& e, const constexpr_constant<T, Value>& cst)
{
    if constexpr (Value == 0)
        return E{e};
    else
        return operation<sub_operation, E, constexpr_constant<T, Value>>{e, cst};
}

template <typename T1, T1 Value1, typename T2, T2 Value2>
constexpr auto operator-(const constexpr_constant<T1, Value1>& cst1, const constexpr_constant<T2, Value2>& cst2)
{
    return constexpr_constant<
        decltype(std::declval<T1>() - std::declval<T2>()), Value1 - Value2>{};
}

#endif /* EXPRESION_H_ */