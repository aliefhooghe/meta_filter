#ifndef META_FILTER_H_
#define META_FILTER_H_

#include <utility>
#include <array>

#include "bilinear_transform/bilinear_transform.h"
#include "expression/evaluate.h"
#include "utils/type_list.h"
#include "utils/variable_set.h"

/**
 * Variable store
 */
template <typename Tag, typename T>
struct variable_association {
    T value{};
};

template <typename T, typename ...Tag>
class variable_store : variable_association<Tag, T>...
{

public:
    constexpr variable_store() = default;

    template <typename Searchtag>
    constexpr auto get() const
    {
        return variable_association<Searchtag, T>::value;
    }

    template <typename Searchtag>
    constexpr void set(const T& val)
    {
        variable_association<Searchtag, T>::value = val;
    }

    template <typename E>
    constexpr auto subsitute_variables(const expression<E>& e) const
    {
        return substitute_var_impl<E, Tag...>::subst(*this, e);
    }

    template <typename E>
    constexpr auto eval(const expression<E>& e)
    {
        return evaluate(subsitute_variables(e));
    }

private:
    
    template <typename E, typename ...Tags>
    struct substitute_var_impl;

    template <typename E>
    struct substitute_var_impl<E>
    {
        static constexpr auto subst(const variable_store&, const expression<E>& e) { return E{e}; }
    };

    template <typename E, typename FirstTag, typename ...LastTags>
    struct substitute_var_impl<E, FirstTag, LastTags...>
    {
        static constexpr auto subst(const variable_store& store, const expression<E>& e)
        {
            const auto tmp_expr = substitute(e, variable<FirstTag>{}, store.get<FirstTag>());
            using tmp_expr_type = std::decay_t<decltype(tmp_expr)>;
            return substitute_var_impl<tmp_expr_type, LastTags...>::subst(store, tmp_expr);
        }
    };
};

/**
 *
 */

template <typename Tztransform>
struct ztransform_info;

template <typename P1, typename P2>
struct ztransform_info<rational_fraction<P1, P2>>
{
    static_assert (P1::degree() <= P2::degree());
    static constexpr auto filter_order = P2::degree();
    using var_tags = variable_set_t<rational_fraction<P1, P2>>;
};

//-

template <typename Tsample, typename Tztransform>
struct iir_filter_implementation;

template <typename Tsample, typename Pnumerator, typename Pdenominator>
struct iir_filter_implementation<Tsample, rational_fraction<Pnumerator, Pdenominator>>
{
    using Tztransform = rational_fraction<Pnumerator, Pdenominator>;
    using info = ztransform_info<Tztransform>;

    //    variable_store_t = variable_store<Tsample, Tags...>  
    using variable_store_t = 
        type_list_instanciate_t<
            type_list_append_t<typename info::var_tags, Tsample>, variable_store>;

public:
    constexpr iir_filter_implementation(const Tztransform& transfert_function)
    :   _transfert_function{transfert_function}
    {}

    Tsample process_one_sample(const Tsample& in)
    {
        const auto output_divider_expr = std::get<Pdenominator::degree()>(_transfert_function.denominator.coefficients);
        const auto output_divider_val = _variable_store.eval(output_divider_expr);

        const auto out = 
            sum_helper(
                std::make_integer_sequence<unsigned int, Pnumerator::degree() + 1>{},
                std::make_integer_sequence<unsigned int, Pdenominator::degree()>{}, in)
            / output_divider_val;

        enqueue(in, out);
        return out;
    }

    template <typename SearchTag>
    constexpr void set_variable(const variable<SearchTag>&, const Tsample& value)
    {
        _variable_store.template set<SearchTag>(value);
    }

private:

    template <unsigned int Idx>
    constexpr auto get_input(const Tsample& current_in)
    {
        if constexpr (Idx == info::filter_order)
            return current_in;
        else
            return _prev_input_queue[Idx];
    }

    template <unsigned int ...FeedforwardIndexes, unsigned int ...FeedbackIndexes>
    constexpr Tsample sum_helper(
        const std::integer_sequence<unsigned int, FeedforwardIndexes...>&,
        const std::integer_sequence<unsigned int, FeedbackIndexes...>&,
        const Tsample& in)
    {
        const auto output_expr = 
            ((... + (std::get<FeedforwardIndexes>(_transfert_function.numerator.coefficients) * get_input<FeedforwardIndexes>(in))) -
            (... + (std::get<FeedbackIndexes>(_transfert_function.denominator.coefficients) * _prev_output_queue[FeedbackIndexes])));

        return
            _variable_store.eval(output_expr);
    }

    constexpr void enqueue(const Tsample& in, const Tsample& out)
    {
        //  TO BE optimized with a circular buffer
        for (auto i = 0; i < (info::filter_order - 1); ++i)
            _prev_input_queue[i] = _prev_input_queue[i + 1];
        for (auto i = 0; i < (info::filter_order - 1); ++i)
            _prev_output_queue[i] = _prev_output_queue[i + 1];
        _prev_input_queue[info::filter_order - 1] = in;
        _prev_output_queue[info::filter_order - 1] = out;
    }

    const Tztransform _transfert_function;
    variable_store_t _variable_store{};
    std::array<Tsample, info::filter_order> _prev_input_queue{};
    std::array<Tsample, info::filter_order> _prev_output_queue{};
};

template <typename Tsample, typename E>
constexpr auto make_filter(const expression<E>& laplace_transfert_function)
{
    const auto z_transfert_function = bilinear_transform(laplace_transfert_function);    
    using z_transform_type = std::decay_t<decltype(z_transfert_function)>;
    return iir_filter_implementation<Tsample, z_transform_type>{z_transfert_function};
}

#endif /* META_FILTER_H_ */