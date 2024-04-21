#pragma once

#include <type_traits>
#include <cstdint>
#include <tuple>
#include <utility>

#include "../Types/Types.h"

namespace Utility {
 
    template <typename... Types> 
    struct type_list {
    private:
        template <typename Fn, size_t... Indices>
        static auto Iterate(Fn&& fn, std::index_sequence<Indices...>) {
            // Using a fold expression to invoke the lambda function on each type in the type_list.
            return (fn.template operator()<std::tuple_element_t<Indices, std::tuple<Types...>>>(), ...);
        }
    public: 

        template <typename Fn>
        static auto Iterate(Fn&& fn) {
            return Iterate(std::forward<Fn>(fn), std::make_index_sequence<sizeof...(Types)>());
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
        
        /**
         * @brief 0 Based indexing
         * @tparam N index of type
         */
        template <size_t N>
        using Type = typename std::tuple_element_t<N, std::tuple<Types...>>;

        /**
         * @brief Generate a new type_iterator alias for a list of types. 
         * 
         * @tparam SubTypes types must already be present in the type_iterator for them to be a subset of the type_iterator
         */
        template <typename... SubTypes>
        using Subset = std::enable_if_t<iterator::template HasAll<SubTypes...>(), type_iterator<SubTypes...>>;

        static const constexpr size_t Count = sizeof...(Types);

         /**
         * @brief Iterate over an empty instance of all types in the iterator.
         * 
         * @tparam Fn Templated Lambda function -> [=|&|None]<typename>(){}; 
         * @tparam T Labmda param, the current type in the iteration.
         * 
         * @param fn Function to be called on each iteration.
         * 
         * @param Example -> @code auto result = type_iterator<Types...>::Iterate([]<typename T>() {
         *      T::StaticMethod();
         *      TemplateFunction<T>();
         *      return Something; 
         *  }); @endcode
         * 
         * @result User defined return type. See 'type_list private: Iterate' above for more details 
         */
        template <typename Fn>
        static auto Iterate(Fn&& fn) {
            return iterator::Iterate(std::forward<Fn>(fn));
        }
        /**
         * @brief Iterate over an empty instance of all types not excluded that are present in the iterator.
         * 
         * @tparam Fn Templated Lambda function -> [=|&|None]<typename>(){};
         * @tparam T Labmda param, the current type in the iteration.
         * @tparam Exclude first type to be excluded
         * @tparam ...Exclusions the rest of the types to be excluded. 
         * 
         * @param fn Function to be called on each iteration.
         * 
         * @param Example -> @code auto result = type_iterator<Types...>::Iterate([&]<typename T>() {
         *      T::StaticMethod();
         *      TemplateFunction<T>();
         *      return Something; 
         *  }); @endcode
         * 
         * @result User defined return type. See 'type_list private: Iterate' above for more details 
         */
        template <typename Exclude, typename... Exclusions, typename Fn>
        static auto Iterate(Fn&& fn) {
            static_assert(HasAll<Exclude, Exclusions...>(), "type_iterator does not contain excluded types");
            using iterables = typename remove_types_from_list<type_list<Exclude, Exclusions...>, iterator>::type;
            return iterables::Iterate(std::forward<Fn>(fn));
        }
        

        template <typename T>
        constexpr static bool Has() { return iterator::template Has<T>();}

        template <typename First, typename Second, typename... Rest>
        constexpr static bool HasAny() { return iterator::template HasAny<First, Second, Rest...>();}

        template <typename First, typename Second, typename... Rest>
        constexpr static bool HasAll() { return iterator::template HasAll<First, Second, Rest...>();}

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