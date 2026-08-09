#pragma once
#include <type_traits>

namespace bee::tmp
{
    template <typename... Ts>
    struct type_list;
    
    template <typename T>
    struct is_type_list;
    
    template <typename List, typename T>
    struct push_front;

    template <typename List, typename T>
    struct push_back;

    template <typename List>
    struct pop_front;

    template <typename List>
    struct pop_back;

    template <typename A, typename B>
    struct same_type_list;
    
    template <typename List>
    struct type_list_size;

    template <typename... Lists>
    struct concat_many;
    
    template <typename List, template <typename> typename F>
    struct transform;

    template <typename T, typename List>
    struct exist;

    template <typename List>
    struct unique;

    template <typename List>
    struct size;
    
    template <typename T, typename List>
    struct find;

    template <typename List, template <typename> typename Pred>
    struct find_if;
    
    template <typename List, template <typename> typename Pred>
    struct count_if;

    template <typename List, std::size_t I>
    struct at;

    template <typename List>
    struct front;

    template <typename List>
    struct back;

    template <typename List, template <typename> typename Pred>
    struct filter;

    template <typename List, std::size_t Index, typename New>
    struct replace_at;

    template <typename ListA, typename ListB>
    struct is_inclusive;
    
    template <typename ListA, typename ListB>
    struct is_intersective;
    
    template <typename List>
    struct to_tuple;
}