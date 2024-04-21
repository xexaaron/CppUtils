#include <iostream>
#include "TypeIterator.h"

using namespace Utility;

int main() {
    // Declare an alias for your type_iterator
    using Iterables = type_iterator<int, float, double, char, size_t>;
    
    auto foo = Iterables::Type<0>(0.0);

    auto foo = Iterables::Iterate([]<typename T>(){
        std::cout << GetTypename<T>() << std::endl;
        size_t count = Iterables::Count;
        
        return "5";
    });

    std::cout << foo;
}