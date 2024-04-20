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

    #pragma endregion


    /**
     * @brief Iterator for iterating a variadic number of types.
     * 
     * @tparam ...Types List of types to be iterable.
     * 
     * @note Essentially a wrapper for type_list with extended functionality. like exclusions, and stringifying.
    */
    template <typename... Types>
    class type_iterator {
    private:
        using iterator = type_list<Types...>;
    public:
        using First = typename iterator::First;
        using Last = typename iterator::Last;

       template <typename... SubTypes>
       using Subset = std::enable_if_t<iterator::template HasAll<SubTypes...>(), type_iterator<SubTypes...>>;

        
        /**
         * @brief Iterate over an empty instance of all types in the iterator.
         * 
         * @tparam Fn Templated Lambda function -> []<>(){}; 
         * 
         * @param fn Function to be called on each iteration.
         * 
         * @param Example -> @code type_iterator<Types...>::IterateAll([]<typename T>() {
         *      T::StaticMethod();
         *      TemplateFunction<T>();
         *  }); @endcode
         * 
         */
        template <typename Fn>
        static void IterateAll(Fn&& fn) {
            iterator::Iterate(std::forward<Fn>(fn));
        }

        /**
         * @brief Iterate over an empty instance of all types not excluded by the template parameters.
         * 
         * @tparam Fn Templated Lambda function -> []<>(){}; 
         * @tparam ...Exclusions List of types to be excluded
         * 
         * @param fn Function to be called on each iteration.
         * 
         * @param Example -> @code type_iterator<Types...>::IterateExcluding<Exclusions...>([]<typename T>() {
         *      T::StaticMethod();
         *      TemplateFunction<T>();
         *  }); @endcode
        */
        template <typename... Exclusions, typename Fn>
        static void IterateExcluding(Fn&& fn) {
            static_assert(HasAll<Exclusions...>(), "type_iterator does not contain excluded types");
            using iterables = typename remove_types_from_list<type_list<Exclusions...>, iterator>::type;
            iterables::Iterate(std::forward<Fn>(fn));
        }

        /**
         * @brief Iterate over an empty instance of all types specifically included in the template parameters.
         * 
         * @tparam Fn Templated Lambda function -> []<>(){}; 
         * @tparam ...Inclusions List of types to be iterated over.
         * 
         * @param fn Function to be called on each iteration.
         *  
         * @param Example -> @code type_iterator<Types...>::IterateOver<Inclusions...>([]<typename T>() {
         *      T::StaticMethod();
         *      TemplateFunction<T>();
         *  }); @endcode
        */
        template <typename... Inclusions, typename Fn>
        static void IterateOver(Fn&& fn) {
            static_assert(HasAll<Inclusions...>(), "type_iterator does not contain included types");
            type_list<Inclusions...>::Iterate(std::forward<Fn>(fn));
        }

        /**
         * @brief Get the number of types that are iterable.
         * 
         * @return constexpr size_t 
         */
        constexpr static size_t Count() { return iterator::Count(); }

        /**
         * @brief Check if the iterator contains a type.
         * 
         * @tparam T Type to check.
         * 
         * @return bool
         */
        template <typename T>
        constexpr static bool Has() { return iterator::template Has<T>();}

        /**
         * @brief Check if an iterator contains any type provided.
         * @tparam Variadic list of types
         * @return bool 
        */
        template <typename First, typename Second, typename... Rest>
        constexpr static bool HasAny() { return iterator::template HasAny<First, Second, Rest...>();}
        
        /**
         * @brief Check if an iterator contains all types provided.
         * @tparam Variadic list of types
         * @return bool 
        */
        template <typename First, typename Second, typename... Rest>
        constexpr static bool HasAll() { return iterator::template HasAll<First, Second, Rest...>();}

        /**
         * @brief Get all types in a string format.
         * 
         * @param just_types Boolean if you want to remove the type_list<> part from the string
         * and just have a result of int, float, ..., etc. 
         * 
         * @return std::string -> "type_list<int, float, ...>"
         */
        static std::string String(bool just_types = false) {
            std::string out;
            IterateAll([&out]<typename T>(){
                out += GetTypename<T>(); 
                if (!std::is_same_v<T, Last>) {
                    out += ",";
                }
            });
            if (!just_types) {
                out = "type_list<" + out;
                out += ">";
            }
            return out; 
        }

    };
}