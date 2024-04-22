#pragma once

#include <concepts>

/* CAre*<Types...> Pattern :
        template <typename T> 
        concept CAre* = (... && std::is_*<Types>);
*/


template <template <typename> typename TypeTrait, typename... Types>
concept CAreConcept = (... && TypeTrait<Types>::value);

template <typename... Types>
concept CAreDefaultConstructible = CAreConcept<std::is_default_constructible, Types...>;

template <typename... Types>
constexpr bool are_default_constructible_v = (... && std::is_default_constructible_v<Types>);

template <typename... Types>
concept CArePointers = CAreConcept<std::is_pointer, Types...>;




