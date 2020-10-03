#ifndef BILINEAR_TRANSFORM_UTILS_H_
#define BILINEAR_TRANSFORM_UTILS_H_

#include <utility>

/*
 *  integer_sequence_cat 
 */

template <typename Seq1, typename Seq2>
struct integer_sequence_cat_impl;

template <typename Seq1, typename Seq2>
using integer_sequence_cat = 
    typename integer_sequence_cat_impl<Seq1, Seq2>::type;

template <typename T, T ...I1s, T ...I2s>
struct integer_sequence_cat_impl<std::integer_sequence<T, I1s...>, std::integer_sequence<T, I2s...>>
{
    using type = 
        std::integer_sequence<T, I1s..., I2s...>;
};

/*
 *  integer_range_impl 
 */

template <typename T, T Begin, T End>
struct integer_range_impl
{
    static_assert(End > Begin);
    using type = 
        integer_sequence_cat<
            std::integer_sequence<T, Begin>,
            typename integer_range_impl<T, Begin + 1, End>::type
        >;
};

template <typename T, T End>
struct integer_range_impl<T, End, End>
{
    using type = std::integer_sequence<T>;
};

template <typename T, T Begin, T End>
using integer_range = 
    typename integer_range_impl<T, Begin, End>::type;

#endif /* BILINEAR_TRANSFORM_UTILS_H_ */