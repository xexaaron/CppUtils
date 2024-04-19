#include <iostream>
#include "../../Types/TypeIterator/TypeIterator.h"

using namespace Utility;

int main() {
    using Iterables = TypeIterator<int, float, double, char, size_t>;

    int in = 0;

    if (Iterables::Has<int>()) { // if int
            Iterables::Iterate<int>([&in]<typename T>() { // exclude int and pass as param instead and...
            std::cout << "Type: " << typeid(T).name() << "\n"; 
            if (std::is_same_v<T, Iterables::Last>) { // ...use int on the last iteration of the function
                std::cout << "Type: " << typeid(decltype(in)).name() << "\n"; 
            } 
        });
    }
    // expected output :
    // Type f : float
    // Type d : double
    // Type c : char
    // Type y : size_t
    return 0;
}