#pragma once

#include <type_traits>
#include <cstdint>
#include <tuple>
#include <utility>


namespace Utility {
    /* Extended beyond original implementation */
    #pragma region https://stackoverflow.com/questions/67347545/compile-time-set-subtraction-of-two-tuples

    /**
     * @brief List of types
    */
    template <typename... Pack> 
    struct ClassList {
    private: // Edited ClassList to add Iterate and Count
        template <typename Fn, size_t... Indices>
        static void Iterate(Fn&& fn, std::index_sequence<Indices...>) {
            (std::forward<Fn>(fn).template operator()<std::tuple_element_t<Indices, std::tuple<Pack...>> >(), ...);
        }

    public:
        constexpr static size_t Count() { return sizeof...(Pack); }

        template <typename Fn>
        static void Iterate(Fn&& fn) {
            Iterate(std::forward<Fn>(fn), std::make_index_sequence<sizeof...(Pack)>());
        }

        template <typename T>
        constexpr static bool Has() {
            bool result = false;
            ((result |= std::is_same_v<T, Pack>), ...);
            return result;
        }

        using First = std::tuple_element_t<0, std::tuple<Pack...>>;
        using Last = std::tuple_element_t<sizeof...(Pack) - 1, std::tuple<Pack...>>;
    };

    template<typename...>
    struct Join {
    };
    template<typename... Pack1, typename... Pack2>
    struct Join<ClassList<Pack1...>, ClassList<Pack2...>> {
        using Type = ClassList<Pack1..., Pack2...>;
    };
    template<typename...>
    struct RemoveSingleTypeFromList {
    };
    template<typename Target, typename... Pack>
    struct RemoveSingleTypeFromList<Target, ClassList<Pack...>> {
        using Type = ClassList<Pack...>;
    };
    template<typename Target, typename Parameter, typename... Pack>
    struct RemoveSingleTypeFromList<Target, ClassList<Parameter, Pack...>> {
        using Type = typename Join<
            std::conditional_t<
            std::is_same_v<Target, Parameter>,
            ClassList<>,
            ClassList<Parameter>
            >,
            typename RemoveSingleTypeFromList<Target, ClassList<Pack...>>::Type
        >::Type;
    };
    template<typename... Pack>
    struct RemoveTypesFromList {
    };
    template<typename... Types>
    struct RemoveTypesFromList<ClassList<>, ClassList<Types...>> {
        using Type = ClassList<Types...>;
    };
    template<typename Target, typename... RemainingTargets, typename... Types>
    struct RemoveTypesFromList<ClassList<Target, RemainingTargets...>, ClassList<Types...>> {
        using Type = typename RemoveSingleTypeFromList<
            Target,
            typename RemoveTypesFromList<
            ClassList<RemainingTargets...>,
            ClassList<Types...>
            >::Type
        >::Type;
    };

    #pragma endregion

    /**
     * @brief Iterator for iterating a variadic number of types.
     * @tparam ...Types List of types to be iterable.
    */
    template <typename... Types>
    class TypeIterator {
    private:
        using Iterator = ClassList<Types...>;

    public:
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
            using Iterables = typename RemoveTypesFromList<ClassList<Exclusions...>, Iterator>::Type;
            // Forward the function to Iterables::Iterate
            Iterables::Iterate(std::forward<Fn>(fn));
        }
        /**
         * @brief Get the number of types that are iterable.
         * 
         * @return constexpr size_t 
         */
        constexpr static size_t Count() { return Iterator::Count(); }

        template <typename T>
        constexpr static bool Has() { return Iterator::template Has<T>();}

        using First = typename Iterator::First;
        using Last = typename Iterator::Last;
    };
}