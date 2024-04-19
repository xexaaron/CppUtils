#pragma once
#include <tuple>

namespace Utility {
    template <typename T, typename Tuple>
    struct is_in_tuple;

    template <typename T, typename... Types>
    struct is_in_tuple<T, std::tuple<Types...>> : std::disjunction<std::is_same<T, Types>...> {};

    template <typename T, typename... Types>
    using tuple_has_t = is_in_tuple<T, Types...>;
}

