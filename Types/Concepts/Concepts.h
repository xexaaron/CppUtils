#if __cplusplus == 202002L
    #include <concepts>

    /* CAre*<Types...> Pattern :
            template <typename T> 
            concept CAre* = (... && std::is_*<Types>);
    */

    template <typename... Types>
    concept CAreDefaultConstructible = (... && std::is_default_constructible_v<Types>);
#else 

    template <typename... Types>
    constexpr bool are_default_constructible = (... && std::is_default_constructible_v<Types>);

#endif // CPP20