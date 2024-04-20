#pragma once

#include <type_traits>
#include <cstdint>
#include <tuple>
#include <utility>

#include "../Types.h"

namespace Utility {
    /* Extended beyond original implementation 
       & Renamed some stuff to better match the std library */
    #pragma region https://stackoverflow.com/questions/67347545/compile-time-set-subtraction-of-two-tuples

    /**
     * @brief List of types
    */
    template <typename... Types> 
    struct type_list {
    private: // Edited ClassList to add Iterate and Count
        template <typename Fn, size_t... Indices>
        static void Iterate(Fn&& fn, std::index_sequence<Indices...>) {
            (std::forward<Fn>(fn).template operator()<std::tuple_element_t<Indices, std::tuple<Types...>> >(), ...);
        }

    public:
        constexpr static size_t Count() { return sizeof...(Types); }

        template <typename Fn>
        static void Iterate(Fn&& fn) {
            Iterate(std::forward<Fn>(fn), std::make_index_sequence<sizeof...(Types)>());
        }

        template <typename T>
        constexpr static bool Has() {
            bool result = false;
            ((result |= std::is_same_v<T, Types>), ...);
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

    #pragma endregion

    /**
     * @brief Iterator for iterating a variadic number of types.
     * @tparam ...Types List of types to be iterable.
    */
    template <typename... Types>
    class type_iterator {
    private:
        using iterator = type_list<Types...>;
    public:
        using First = typename iterator::First;
        using Last = typename iterator::Last;

        /**
         * @brief Iterate over an empty instance of each type.
         * 
         * @tparam Fn Templated Lambda function -> []<>(){}; 
         * @tparam ...Exclusions Types to be excluded from iteration. Leaving parameter as empty means that all types will be iterated.
         * 
         * @note Example - @code TypeIterator<Types...>::Iterate<Exclusions...>([]<typename T>() {
         *      T::StaticMethod();
         *      TemplateFunction<T>();
         *  }); @endcode
         * 
         * @param fn Function to be called on each iteration.
         */
        template <typename... Exclusions, typename Fn>
        static void Iterate(Fn&& fn) {
            // Determine the list of types to iterate over, excluding specified types
            using iterables = typename remove_types_from_list<type_list<Exclusions...>, iterator>::type;
            // Forward the function to Iterables::Iterate
            iterables::Iterate(std::forward<Fn>(fn));
        }
        /**
         * @brief Get the number of types that are iterable.
         * 
         * @return constexpr size_t 
         */
        constexpr static size_t Count() { return iterator::Count(); }

        template <typename T>
        constexpr static bool Has() { return iterator::template Has<T>();}

        static std::string TypeString() {
            std::string out;
            out += "type_list<";
            Iterate([&out]<typename T>(){
                out += GetTypename<T>(); 
                if (!std::is_same_v<T, Last>) {
                    out += ",";
                }
            });
            out += ">";
            return out; 
        }

    };
}