#include <iostream>
#include "../../Types/TypeIterator/TypeIterator.h"

using namespace Utility;

int main() {
    using Iterables = type_iterator<int, float, double, char, size_t>;

    int in = 0;

    if (Iterables::Has<int>()) { // if int
            Iterables::Iterate<int>([&in]<typename T>() { // exclude int and pass as param instead and...
            std::cout << "Type: " << GetTypename<T>() << "\n"; 
            if (std::is_same_v<T, Iterables::Last>) { // ...use int on the last iteration of the function
                std::cout << "Type: " << GetTypename<decltype(in)>() << "\n"; 
            } 
        });
        // expected output :
        // Type: float
        // Type: double
        // Type: char
        // Type: size_t
        // Type: int
    }
    
    std::cout << std::endl;

    std::cout << "Types List Printing" << std::endl;
    std::cout << Iterables::TypeString() << std::endl;

    return 0;
}