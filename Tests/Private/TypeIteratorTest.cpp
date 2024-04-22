#include <iostream>

#include "../../Types/TypeIterator/TypeIterator.h"

using namespace Utility;

#define SPACER std::cout << "---------------------------------------" << std::endl

#define LOG_RESULT(expression, x) \
    do { \
        if (expression) { \
            std::cout << std::endl; \
            std::cout << "\033[32m" << x << "\nSucceeded\033[0m" << std::endl; /* Green color for true */ \
        } else { \
            std::cout << std::endl; \
            std::cout << "\033[31m" << x << "\nFailed \033[0m" << std::endl; /* Red color for false */ \
        } \
    } while (0)

#define LOG_HEADER(x) \
    do { \
        SPACER; \
        std::cout << "\033[35m" << x << "\033[0m" << std::endl; \
        SPACER; \
    } while (0)

int main() {
    // Declare an alias for your type_iterator
    using Iterables = type_iterator<int, float, double, char, size_t>;

    {
        LOG_HEADER("Normal iteration over all types");


        bool passed = true;
        std::string expected_type_array[5] = {
            "int", "float", "double", "char", "unsigned long long"
        };
        size_t i = 0;

        Iterables::Iterate([&]<typename T>() { // Normal iteration over all types
            std::cout << "Type: " << GetTypename<T>() << "\n"; 
            passed = GetTypename<T>() == expected_type_array[i++];
        });
        
        LOG_RESULT(passed, "Expected : int, float, double, char, unsigned long long");
    }




    {
        LOG_HEADER("Exclusive iteration");
        
        bool passed = true;
        std::string expected_type_array[2] = {
            "int", "unsigned long long" 
        };
        size_t i = 0;


        Iterables::Iterate<float, double, char>([&]<typename T>() { // Exclusive iteration
            std::cout << "Type: " << GetTypename<T>() << "\n"; 
            passed = GetTypename<T>() == expected_type_array[i++];
        });

       LOG_RESULT(passed, "Expected : int, unsigned long long");
    }

    {
        LOG_HEADER("Subset iteration over all types");
        
        bool passed = true;
        std::string expected_type_array[2] = {
            "unsigned long long", "int"
        };
        size_t i = 0;
        Iterables::Subset<unsigned long long, int>::Iterate([&]<typename T>() { // Subset iteration over all types in the subset
            std::cout << "Type: " << GetTypename<T>() << "\n"; 
            passed = GetTypename<T>() == expected_type_array[i++];
        });
    
        LOG_RESULT(passed, "Expected : unsigned long long, int");
    }

    {
        LOG_HEADER("type_iterator::String(false)");

        std::cout << Iterables::String(false) << std::endl;
        std::string expected = "type_list<int, float, double, char, unsigned long long>";

        LOG_RESULT(expected == Iterables::String(), "Expected string : type_list<int, float, double, char, unsigned long long>");
    }
    
    {
        LOG_HEADER("type_iterator::String(true)");
        
        std::cout << Iterables::String(true) << std::endl;

        std::string expected = "int, float, double, char, unsigned long long";
        LOG_RESULT(expected == Iterables::String(true), "Expected string : int, float, double, char, unsigned long long");
    }

    {
        LOG_HEADER("PrimitiveTypeIteratorEx");

        PrimitiveTypeIteratorEx::Iterate([]<typename T>(){
            std::cout << GetTypename<T>() << std::endl;
        });

        LOG_RESULT(true, "");
    }

    return 0;
}
