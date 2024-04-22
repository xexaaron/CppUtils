#pragma once

#include <type_traits>
#include <cstdint>
#include <tuple>
#include <utility>

#include "../Types.h"

namespace Utility {

    // based on https://stackoverflow.com/questions/67347545/compile-time-set-subtraction-of-two-tuples

    template <typename... Types> 
    struct type_list {
    private:
        template <typename Fn, size_t... Indices>
        static auto Iterate(Fn&& fn, std::index_sequence<Indices...>) {
            return (fn.template operator()<std::tuple_element_t<Indices, std::tuple<Types...>>>(), ...);
        }
    public: 
        
        template <typename Fn>
        static auto Iterate(Fn&& fn) {
            return Iterate(std::forward<Fn>(fn), std::make_index_sequence<sizeof...(Types)>());
        }
        /* return (using RT = decltype (fn.template operator()<std::tuple_element_t<Indices, std::tuple<Types...>>>(), ...),static RT result, if (!break_loop) RT result = decltype (fn.template operator()<std::tuple_element_t<Indices, std::tuple<Types...>>>(), ...))*/
        template <typename T>
        constexpr static bool Has() {
            bool result = false;
            ((result |= std::is_same_v<T, Types>), ...);
            return result;
        }

        template <typename First, typename Second, typename... Rest>
        constexpr static bool HasAny() {
            bool result = false;
            result |= Has<First>();
            result |= Has<Second>();
            ((result |= Has<Rest>()), ...);
            return result;
        }

        template <typename First, typename Second, typename... Rest>
        constexpr static bool HasAll() {
            bool result = true;
            result = result && Has<First>();
            result = result && Has<Second>();
            ((result = result && Has<Rest>()), ...);
            return result;
        }

        using First = std::tuple_element_t<0, std::tuple<Types...>>;
        using Last = std::tuple_element_t<sizeof...(Types) - 1, std::tuple<Types...>>;
    };

    template<typename...>
    struct join {};

    template<typename... Types, typename... Other>
    struct join<type_list<Types...>, type_list<Other...>> { using type = type_list<Types..., Other...>; };

    template<typename...>
    struct remove_t_from_list {};

    template<typename Target, typename... Types>
    struct remove_t_from_list<Target, type_list<Types...>> { using type = type_list<Types...>; };
    
    template<typename Target, typename Parameter, typename... Types>
    struct remove_t_from_list<Target, type_list<Parameter, Types...>> {
        using type = typename join<
            std::conditional_t<
            std::is_same_v<Target, Parameter>,
            type_list<>,
            type_list<Parameter>
            >,
            typename remove_t_from_list<Target, type_list<Types...>>::type
        >::type;
    };

    template<typename... Types>
    struct remove_types_from_list {};
    
    template<typename... Types>
    struct remove_types_from_list<type_list<>, type_list<Types...>> { using type = type_list<Types...>; };
    
    template<typename Target, typename... RemainingTargets, typename... Types>
    struct remove_types_from_list<type_list<Target, RemainingTargets...>, type_list<Types...>> {
        using type = typename remove_t_from_list<
            Target,
            typename remove_types_from_list<
            type_list<RemainingTargets...>,
            type_list<Types...>
            >::type
        >::type;
    };
}