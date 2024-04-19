#include <iostream>
#include "../../Types/TypeIterator/TypeIterator.h"

using namespace Utility;

int main() {
    using Iterables = TypeIterator<int, float, double, char, size_t>;

    if (Iterables::Has<int>()) { // if int
            Iterables::Iterate<int>([]<typename T>() { // exclude int
            std::cout << "Type: " << typeid(T).name() << "\n"; 
        });
    }
    // expected output :
    // Type f : float
    // Type d : double
    // Type c : char
    // Type y : size_t
    return 0;
}