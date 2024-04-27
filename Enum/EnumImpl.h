#pragma once

/* File should not be included by files other than Enum.h */

#include "Helpers.h"

#define ENUM(name) _##name
#include <type_traits>

#include "../Types/Types.h"

#define CREATE_ENUM(eName, eType, ...)                                                                                         \
    struct alignas(eType) eName                                                                                                \
    {                                                                                                                          \
        static_assert(Utility::BeginsWith<'E'>(#eName), "Advanced enum structures are prefixed with 'E' to avoid confusion."); \
        enum _##eName : eType{                                                                                                 \
            __VA_ARGS__,                                                                                                       \
            ENULL = std::numeric_limits<eType>::max()};                                                                        \
        inline eName() noexcept : _Value(eName::ENULL) {}                                                                      \
        inline eName(_##eName value) noexcept : _Value(value) {}                                                               \
        inline eName(eType value) noexcept : _Value(static_cast<_##eName>(value)) {}                                           \
        inline eName(bool value) noexcept : _Value(static_cast<_##eName>(static_cast<eType>(value))) {}                        \
        inline eName(const eName &other) noexcept = default;                                                                   \
        inline eName(eName &&other) = delete;                                                                                  \
        inline constexpr static eType Length() noexcept { return Utility::CountArgs(__VA_ARGS__) + 1; }                        \
        inline operator _##eName &() noexcept { return _Value; }                                                               \
        inline operator const _##eName &() const noexcept { return _Value; }                                                   \
        [[nodiscard]] inline const static std::string ToString(const eName &value) noexcept                                    \
        {                                                                                                                      \
            if (value == _##eName::ENULL)                                                                                      \
            {                                                                                                                  \
                return "ENULL";                                                                                                \
            }                                                                                                                  \
            constexpr static size_t length = Length();                                                                         \
            const static std::vector<std::string> arr = Utility::CreateStringVecUsingDelimiter<length>(#__VA_ARGS__, ',');     \
            return arr[value];                                                                                                 \
        }                                                                                                                      \
        [[nodiscard]] inline const std::string String() const noexcept                                                         \
        {                                                                                                                      \
            if (_Value == _##eName::ENULL)                                                                                     \
            {                                                                                                                  \
                return "ENULL";                                                                                                \
            }                                                                                                                  \
            constexpr static size_t length = Length();                                                                         \
            const static std::vector<std::string> arr = Utility::CreateStringVecUsingDelimiter<length>(#__VA_ARGS__, ',');     \
            return arr[_Value];                                                                                                \
        }                                                                                                                      \
        inline static void Iterate(void (*function)(eName)) noexcept                                                           \
        {                                                                                                                      \
            constexpr size_t length = Length();                                                                                \
            const static std::array<_##eName, length> arr = {__VA_ARGS__};                                                     \
            for (size_t i = 0; i < length - 1; i++)                                                                            \
            {                                                                                                                  \
                function(static_cast<eName>(arr[i]));                                                                          \
            }                                                                                                                  \
        }                                                                                                                      \
        template <typename T>                                                                                                  \
        inline static void Iterate(T function) noexcept                                                                        \
        {                                                                                                                      \
            constexpr size_t length = Length();                                                                                \
            const static std::array<_##eName, length> arr = {__VA_ARGS__};                                                     \
            for (size_t i = 0; i < length - 1; i++)                                                                            \
            {                                                                                                                  \
                function(static_cast<eName>(arr[i]));                                                                          \
            }                                                                                                                  \
        }                                                                                                                      \
        [[nodiscard]] inline static std::string Name() noexcept { return #eName; }                                             \
        [[nodiscard]] inline bool IsNull() const noexcept { return _Value == _##eName::ENULL; }                                \
        [[nodiscard]] inline constexpr static eType Max() noexcept { return static_cast<eType>(_##eName::ENULL) - 1; }         \
        [[nodiscard]] inline friend std::ostream &operator<<(std::ostream &os, const eName &enumObj) noexcept                  \
        {                                                                                                                      \
            return os << enumObj.String();                                                                                     \
        }                                                                                                                      \
        inline friend eName operator&(const eName &lhs, const eName &rhs) noexcept                                             \
        {                                                                                                                      \
            return eName(static_cast<eType>(lhs._Value & rhs._Value));                                                         \
        }                                                                                                                      \
        inline friend eName operator|(const eName &lhs, const eName &rhs) noexcept                                             \
        {                                                                                                                      \
            return eName(static_cast<eType>(lhs._Value | rhs._Value));                                                         \
        }                                                                                                                      \
        inline friend eName operator^(const eName &lhs, const eName &rhs) noexcept                                             \
        {                                                                                                                      \
            return eName(static_cast<eType>(lhs._Value ^ rhs._Value));                                                         \
        }                                                                                                                      \
        inline friend eName operator~(const eName &value) noexcept                                                             \
        {                                                                                                                      \
            return eName(static_cast<eType>(~value._Value));                                                                   \
        }                                                                                                                      \
                                                                                                                               \
    private:                                                                                                                   \
        _##eName _Value = _##eName::ENULL;                                                                                     \
    }
