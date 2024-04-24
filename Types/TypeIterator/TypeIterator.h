#pragma once

#include <type_traits>
#include <cstdint>
#include <tuple>
#include <utility>

#include "../Types.h"
#include "TypeList.h"

namespace Utility {
    
    // See Tests/Private/TypeIteratorTest.cpp for examples of using a type_iterator.

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
        template <size_t N> // 0 based indexing
        using Type = typename std::tuple_element_t<N, std::tuple<Types...>>;
        using AsTuple = typename iterator::AsTuple;

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
            Iterate([&out]<typename T>(){
                out += GetTypename<T>(); 
                if (!std::is_same_v<T, Last>) {
                    out += ", ";
                }
            });
            if (!just_types) {
                out = "type_list<" + out;
                out += ">";
            }
            return out; 
        }
        
    };




    struct PrimitiveTypeIterator : type_iterator<
        void, char, unsigned char, wchar_t, short int, unsigned short int,
        int, unsigned int, long int, unsigned long int, long long int,
        unsigned long long int, bool, float, double, long double> {};

    struct PrimitiveTypeIteratorEx : type_iterator<
        void, char, unsigned char, wchar_t, char16_t, char32_t, std::byte, short int, unsigned short int,
        int, unsigned int, long int, unsigned long int, long long int,
        unsigned long long int, bool, float, double, long double> {};

    struct CharTypeIterator : type_iterator<char, unsigned char, wchar_t> {};

    struct CharTypeIteratorEx : type_iterator<char, unsigned char, wchar_t, char16_t, char32_t> {};

    struct NumericTypeIterator : type_iterator<short int, unsigned short int,
        int, unsigned int, long int, unsigned long int, long long int,
        unsigned long long int, bool, float, double, long double> {};

    struct NumericTypeIteratorEx : type_iterator<std::byte, short int, unsigned short int,
        int, unsigned int, long int, unsigned long int, long long int,
        unsigned long long int, bool, float, double, long double> {};
}