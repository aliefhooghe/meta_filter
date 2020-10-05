#ifndef TYPE_LIST_H_
#define TYPE_LIST_H_

#include <type_traits>

//

template <typename ...T>
struct type_list {};

//

template <typename Tlist, typename T>
struct type_list_contains;

template <typename Tlist, typename T>
constexpr auto type_list_contains_v = type_list_contains<Tlist, T>::value;

template <typename ...Ts, typename T>
struct type_list_contains<type_list<Ts...>, T>
{
    static constexpr auto value = (... || std::is_same_v<Ts, T>);
};

//

template <typename Tlist, typename T>
struct type_list_append;

template <typename Tlist, typename T>
using type_list_append_t = typename type_list_append<Tlist, T>::type;

template <typename ...Ts, typename T>
struct type_list_append<type_list<Ts...>, T>
{
    using type = type_list<T, Ts...>;
};

//

template <typename Tlist, typename T>
struct type_list_set_insert
{
    using type = 
        std::conditional_t<
            type_list_contains_v<Tlist, T>,
            Tlist,
            type_list_append_t<Tlist, T>
        >;
};

template <typename Tlist, typename T>
using type_list_set_insert_t = typename type_list_set_insert<Tlist, T>::type;

//

template <typename ...Tlist>
struct type_list_set_merge;

template <typename ...Tlist>
using type_list_set_merge_t = typename type_list_set_merge<Tlist...>::type;

template <typename Tlist>
struct type_list_set_merge<Tlist>
{
    using type = Tlist;
};

template <typename TFirstList, typename ...TNextLists>
struct type_list_set_merge<TFirstList, type_list<>, TNextLists...>
{
    using type = 
        type_list_set_merge_t<TFirstList, TNextLists...>;
};

template <typename TFirstList, typename Tfirst2, typename ...Tlast2, typename ...TNextLists>
struct type_list_set_merge<TFirstList, type_list<Tfirst2, Tlast2...>, TNextLists...>
{
    using type = 
        type_list_set_merge_t<
            type_list_set_insert_t<TFirstList, Tfirst2>,
            type_list<Tlast2...>,
            TNextLists...
        >;
};

//

template <typename Tlist, template <typename...> typename T>
struct type_list_instanciate;

template <typename Tlist, template <typename...> typename T>
using type_list_instanciate_t = typename type_list_instanciate<Tlist, T>::type;

template <typename ...Ts, template <typename...> typename T>
struct type_list_instanciate<type_list<Ts...>, T>
{
    using type = T<Ts...>;
};

#endif