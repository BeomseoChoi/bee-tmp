#pragma once
#include "bee/_type_list_detail/type_list_fwd.h"
#include <type_traits>
#include <tuple>

namespace bee::tmp
{
    template <typename T>
    struct is_type_list : std::false_type {};

    template <typename... Ts>
    struct is_type_list<type_list<Ts...>> : std::true_type {};

    template <typename T>
    inline constexpr bool is_type_list_v = is_type_list<T>::value;

}