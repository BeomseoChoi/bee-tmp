#pragma once
#include "bee/_type_list_detail/type_list_fwd.h"
#include "bee/_type_list_detail/type_list_traits.h"

namespace bee::tmp
{
    template <typename... Ts>
    struct type_list 
    {
        template <typename T>
        struct push_front
        {
            using type = typename ::bee::tmp::push_front<type_list<Ts...>, T>::type;
        };

        template <typename T>
        struct push_back
        {
            using type = typename ::bee::tmp::push_back<type_list<Ts...>, T>::type;
        };

        struct pop_front
        {
            using type = typename ::bee::tmp::pop_front<type_list<Ts...>>::type;
        };

        struct pop_back
        {
            using type = typename ::bee::tmp::pop_back<type_list<Ts...>>::type;
        };
    };

    template <typename... Ts, typename T>
    struct push_front<type_list<Ts...>, T>
    {
        using type = type_list<T, Ts...>;
    };
    
    template <typename List, typename T>
    using push_front_t = typename push_front<List, T>::type;

    template <typename... Ts, typename T>
    struct push_back<type_list<Ts...>, T>
    {
        using type = type_list<Ts..., T>;
    };
    
    template <typename List, typename T>
    using push_back_t = typename push_back<List, T>::type;

    template <typename T, typename... Ts>
    struct pop_front<type_list<T, Ts...>>
    {
        using type = type_list<Ts...>;    
    };
    
    template <typename List>
    using pop_front_t = typename pop_front<List>::type;
    
    template <typename T>
    struct pop_back<type_list<T>>
    {
        using type = type_list<>;
    };

    template <typename T, typename... Ts>
    struct pop_back<type_list<T, Ts...>>
    {
        using type =
            typename ::bee::tmp::push_front<
                typename ::bee::tmp::pop_back<type_list<Ts...>>::type,
                T
            >::type;
    };

    template <typename List>
    using pop_back_t = typename pop_back<List>::type;
    
    template <typename A, typename B>
    struct same_type_list : std::false_type {};

    template <typename... A, typename... B>
    struct same_type_list<type_list<A...>, type_list<B...>>
        : std::bool_constant<(sizeof...(A) == sizeof...(B)) && (std::is_same_v<A, B> && ...)> {};

    template <typename... Ts>
    struct type_list_size<type_list<Ts...>>
        : std::integral_constant<std::size_t, sizeof...(Ts)> {};

    template <typename A, typename B>
    struct concat_type_list;

    template <typename... A, typename... B>
    struct concat_type_list<type_list<A...>, type_list<B...>> 
    {
        using type = type_list<A..., B...>;
    };

    template <>
    struct concat_many<> 
    {
        using type = type_list<>;
    };

    template <typename L>
    struct concat_many<L> 
    {
        using type = L;
    };

    template <typename A, typename B, typename... Rest>
    struct concat_many<A, B, Rest...> 
    {
        using type = typename concat_many<typename concat_type_list<A, B>::type, Rest...>::type;
    };

    template <typename... Lists>
    using concat_many_t = typename concat_many<Lists...>::type;

    template <typename... Ts, template <typename> typename F>
    struct transform<type_list<Ts...>, F> 
    {
        using type = type_list<typename F<Ts>::type...>;
    };

    template <typename List, template <typename> typename F>
    using transform_t = typename transform<List, F>::type;

    template <typename T>
    struct exist<T, type_list<>> : std::false_type {};

    template <typename T, typename... Ts>
    struct exist<T, type_list<Ts...>> : std::bool_constant<(std::is_same_v<T, Ts> || ...)> {};

    template <typename T, typename List>
    inline constexpr bool exist_v = exist<T, List>::value;

    template <>
    struct unique<type_list<>> : std::true_type {};

    template <typename T>
    struct unique<type_list<T>> : std::true_type {};

    template <typename A, typename B, typename... Ts>
    struct unique<type_list<A, B, Ts...>> 
    {
        static constexpr bool value = (!exist_v<A, type_list<B, Ts...>> && unique<type_list<B, Ts...>>::value);
    };

    template <typename List>
    inline constexpr bool unique_v = unique<List>::value;

    template <typename... Ts>
    struct size<type_list<Ts...>> : std::integral_constant<std::size_t, sizeof...(Ts)> {};

    template <typename List>
    inline constexpr std::size_t size_v = size<List>::value;

    template <std::size_t I, typename T, typename List>
    struct find_impl;

    template <std::size_t I, typename T>
    struct find_impl<I, T, type_list<>> : std::integral_constant<std::size_t, I> {};

    template <std::size_t I, typename T, typename First, typename... Ts>
    struct find_impl<I, T, type_list<First, Ts...>>
    {
        static constexpr std::size_t value = std::is_same_v<T, First> ? I : find_impl<I + 1, T, type_list<Ts...>>::value;
    };

    template <typename T, typename... Ts>
    struct find<T, type_list<Ts...>>
    {
        static constexpr std::size_t value = find_impl<0, T, type_list<Ts...>>::value;
    };

    template <typename T, typename List>
    inline constexpr std::size_t find_v = find<T, List>::value;

    template <std::size_t I, typename List, template <typename> typename F>
    struct find_if_impl;

    template <std::size_t I, template <typename> typename F>
    struct find_if_impl<I, type_list<>, F> : std::integral_constant<std::size_t, I> {};

    template <std::size_t I, typename First, typename... Rest, template <typename> typename F>
    struct find_if_impl<I, type_list<First, Rest...>, F>
    {
        static constexpr std::size_t value = F<First>::value ? I : find_if_impl<I + 1, type_list<Rest...>, F>::value;
    };

    template <typename... Ts, template <typename> typename F>
    struct find_if<type_list<Ts...>, F>
    {
        static constexpr std::size_t value = find_if_impl<0, type_list<Ts...>, F>::value;
    };

    template <typename List, template <typename> typename F>
    inline constexpr std::size_t find_if_v = find_if<List, F>::value;

    template <template <typename> typename Pred>
    struct count_if<type_list<>, Pred> : std::integral_constant<std::size_t, 0>
    {};

    template <typename First, typename... Rest, template <typename> typename Pred>
    struct count_if<type_list<First, Rest...>, Pred> : std::integral_constant<
            std::size_t, (Pred<First>::value ? 1 : 0) + count_if<type_list<Rest...>, Pred>::value
        >
    {};

    template <typename T, typename... Ts>
    struct at<type_list<T, Ts...>, 0>
    {
        using type = T;
    };

    template <typename T, typename...Ts, std::size_t I>
        requires (0 < I)
    struct at<type_list<T, Ts...>, I>
    {
        static_assert(I < size_v<type_list<T, Ts...>>);
        using type = typename at<type_list<Ts...>, I - 1>::type;
    };

    template <typename List, std::size_t I>
    using at_t = typename at<List, I>::type;

    template <typename First, typename... Ts>
    struct front<type_list<First, Ts...>>
    {
        using type = First;
    };

    template <typename List>
    using front_t = typename front<List>::type;

    template <typename T>
    struct back<type_list<T>>
    {
        using type = T;
    };

    template <typename First, typename... Rest>
    struct back<type_list<First, Rest...>>
    {
        using type = typename back<type_list<Rest...>>::type;
    };
    
    template <typename List>
    using back_t = typename back<List>::type;

    template <template <typename> typename Pred>
    struct filter<type_list<>, Pred>
    {
        using type = type_list<>; 
    };

    template <typename First, typename... Rest, template <typename> typename Pred>
    struct filter<type_list<First, Rest...>, Pred>
    {
    private:
        using tail = typename filter<type_list<Rest...>, Pred>::type;

    public:
        using type = std::conditional_t<Pred<First>::value, typename push_front<tail, First>::type, tail>;
    };

    template <typename List, template <typename> typename Pred>
    using filter_t = typename filter<List, Pred>::type;

    template <typename First, typename... Rest, typename New>
    struct replace_at<type_list<First, Rest...>, 0, New>
    {
        using type = type_list<New, Rest...>;
    };

    template <typename First, typename... Rest, std::size_t Index, typename New>
    struct replace_at<type_list<First, Rest...>, Index, New>
    {
    private:
        using tail = typename replace_at<type_list<Rest...>, Index - 1, New>::type;

    public:
        using type = typename push_front<tail, First>::type;
    };

    template <typename List, std::size_t Index, typename New>
    using replace_at_t = typename replace_at<List, Index, New>::type;    

    template <typename... Ts>
    struct is_inclusive<type_list<>, type_list<Ts...>> : std::true_type {};

    template <typename... As, typename ListB>
    struct is_inclusive<type_list<As...>, ListB>
    {
        static constexpr bool value = (exist_v<As, ListB> && ...);
    };

    template <typename ListA, typename ListB>
    inline constexpr bool is_inclusive_v = is_inclusive<ListA, ListB>::value;

    template <typename... Ts>
    struct is_intersective<type_list<>, type_list<Ts...>> : std::false_type {};

    template <typename... As, typename Includer>
    struct is_intersective<type_list<As...>, Includer>
    {
        static constexpr bool value = (exist_v<As, Includer> || ...);
    };

    template <typename ListA, typename ListB>
    inline constexpr bool is_intersective_v = is_intersective<ListA, ListB>::value;

    template <typename... Ts>
    struct to_tuple<type_list<Ts...>>
    {
        using type = std::tuple<Ts...>;
    };

    template <typename List>
    using to_tuple_t = typename to_tuple<List>::type;
}