#pragma once

#include <iostream>   
#include <array>      
#include <cstdint>    
#include <limits>     
#include <sstream>    
#include <functional> 
#include <stdexcept>  
#include <vector>
#include <string>
#include <string_view>

namespace Utility {
    /**
     * @brief Counts the number of args.
     * @tparam Args Variadic arguments to be counted
     * @return constexpr size_t -> Number of args
     */
    template <typename... Args>
    constexpr static size_t CountArgs(Args...)
    {
        return sizeof...(Args);
    }

    /**
     * @brief Check if a string begins with a character at compile time.
     * @tparam C char to check if string begins with
     * @param string string to check
     * @return bool indicating if the string begins with that character
     */
    template <char C>
    constexpr static bool BeginsWith(const char *string)
    {
        return string[0] == C;
    }

    /**
     * @brief Check if a string begins with a character at compile time.
     * @tparam C char to check if string begins with
     * @param string string to check
     * @return bool indicating if the string begins with that character
     */
    template <char C>
    constexpr static bool BeginsWith(const std::string &string)
    {
        return string.at(0) == C;
    }

    /**
     * @brief Create a string array from a string using a delimiter at compile time.
     *
     * @tparam N Size of array.
     * @param input std::string of delimiter seperated values
     * @param delimiter char to denote the next value in the input.
     * @return constexpr std::array<std::string, N> -> string array
     */
    template <size_t N = 2>
    static std::array<std::string, N> CreateStringArrUsingDelimiter(const std::string &input, char delimiter)
    {
        std::array<std::string, N> result;
        std::stringstream ss(input);
        std::string token;

        size_t index = 0;
        while (std::getline(ss, token, delimiter) && index < N)
        {
            // Trim leading and trailing whitespace from the token
            size_t start = token.find_first_not_of(" \t");
            size_t end = token.find_last_not_of(" \t");
            if (start != std::string::npos && end != std::string::npos)
            {
                result[index++] = token.substr(start, end - start + 1);
            }
            else if (start != std::string::npos)
            {
                // Only leading whitespace found
                result[index++] = token.substr(start);
            }
            else if (end != std::string::npos)
            {
                // Only trailing whitespace found
                result[index++] = token.substr(0, end + 1);
            }
            else
            {
                // Token is empty or contains only whitespace
                throw std::invalid_argument("Token is empty or contains only whitespace");
            }
        }

        // If there are remaining tokens in the input string, discard them
        std::string remaining;
        while (std::getline(ss, remaining, delimiter))
        {
        }

        return result;
    }

    /**
     * @brief Create a string array from a string using a delimiter at compile time.
     *
     * @tparam N Size of array.
     * @param input std::string of delimiter seperated values
     * @param delimiter char to denote the next value in the input.
     * @return constexpr std::array<std::string, N> -> string array
     */
    template <size_t N = 2>
    static std::vector<std::string> CreateStringVecUsingDelimiter(const std::string &input, char delimiter)
    {
        std::vector<std::string> result;
        result.reserve(N); // Preallocate memory if using std::vector

        size_t start = 0;
        size_t index = 0;
        size_t end = input.find(delimiter);

        while (end != std::string::npos && index < N)
        {
            std::string_view token(input.c_str() + start, end - start);

            // Trim leading and trailing whitespace
            size_t tokenStart = token.find_first_not_of(" \t");
            size_t tokenEnd = token.find_last_not_of(" \t");
            if (tokenStart != std::string_view::npos && tokenEnd != std::string_view::npos)
            {
                result.push_back(std::string(token.substr(tokenStart, tokenEnd - tokenStart + 1)));
            }
            else if (tokenStart != std::string_view::npos)
            {
                // Only leading whitespace found
                result.push_back(std::string(token.substr(tokenStart)));
            }
            else if (tokenEnd != std::string_view::npos)
            {
                // Only trailing whitespace found
                result.push_back(std::string(token.substr(0, tokenEnd + 1)));
            }

            start = end + 1;
            end = input.find(delimiter, start);
            ++index;
        }

        // Add the last token if any

        if (start < input.length() && index < N)
        {
            std::string_view lastToken(input.c_str() + start, input.length() - start);

            // Trim leading and trailing whitespace
            size_t tokenStart = lastToken.find_first_not_of(" \t");
            size_t tokenEnd = lastToken.find_last_not_of(" \t");
            if (tokenStart != std::string_view::npos && tokenEnd != std::string_view::npos)
            {
                result.push_back(std::string(lastToken.substr(tokenStart, tokenEnd - tokenStart + 1)));
            }
            else if (tokenStart != std::string_view::npos)
            {
                // Only leading whitespace found
                result.push_back(std::string(lastToken.substr(tokenStart)));
            }
            else if (tokenEnd != std::string_view::npos)
            {
                // Only trailing whitespace found
                result.push_back(std::string(lastToken.substr(0, tokenEnd + 1)));
            }
        }

        return result;
    }

}
