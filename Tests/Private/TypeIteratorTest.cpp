#include <iostream>
#include "../../Types/TypeIterator/TypeIterator.h"

using namespace Utility;

int main() {
    // Declare an alias for your type_iterator
    using Iterables = type_iterator<int, float, double, char, size_t>;

    std::cout << std::endl;

    std::cout << Iterables::String() << std::endl << std::endl;

    // Iterate all types in the list
    std::cout << "type_iterator::IterateAll : " << std::endl << std::endl;
    Iterables::IterateAll([]<typename T>() { 
        std::cout << "Type: " << GetTypename<T>() << "\n"; 
    });
    
    std::cout << std::endl;

    std::cout << "type_iterator::IterateOver<float, double, char> : " << std::endl << std::endl;
    Iterables::IterateOver<float, double, char>([]<typename T>() { 
        std::cout << "Type: " << GetTypename<T>() << "\n"; 
    });

    std::cout << std::endl; 

    std::cout << "type_iterator::IterateExcluding<float, double, char> : " << std::endl << std::endl;
    Iterables::IterateExcluding<float, double, char>([]<typename T>() { 
        std::cout << "Type: " << GetTypename<T>() << "\n"; 
    });

    std::cout << std::endl;

    std::cout << "Types List Printing : " << std::endl << std::endl;
    std::cout << "just_types = false  : " << Iterables::String() << std::endl;
    std::cout << "just_types = true   : " << Iterables::String(true) << std::endl;

     std::cout << std::endl;

    std::cout << "type_iterator::Subset<unsigned long long, int>::IterateAll " << std::endl << std::endl;
    Iterables::Subset<unsigned long long, int>::IterateAll([]<typename T>() { 
        std::cout << "Type: " << GetTypename<T>() << "\n"; 
    });


    return 0;
}