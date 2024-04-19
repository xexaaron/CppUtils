#pragma once

#include <iostream>

namespace Utility {
    /** @brief Null class with no members
     *  @return T{}; 
    */
    class Null {
        template <typename T>
        operator T() const { return T{}; }
    };

    /**
     * @brief Get the typename of an obj optionally filtered down to just the class/struct name
     *
     * @tparam T Object type
     *
     * @param verbosity 0 -> No namespace and no 'class' or 'struct' keyword.
     * @param verbosity 1 -> Namespace Included no 'class or 'struct' keyword.
     * @param verbosity 2 -> Normal typeid(T).name() includes namespace.
     * @param Tip -> derefrence pointers to achieve less verbose types.
     * 
     * @return std::string 
    */
    template <typename T>
    static std::string GetTypename(unsigned int verbosity = 0) {
        std::string type_name = typeid(T).name();

        switch (verbosity) {
            /* No namespace and no 'class' or 'struct' keyword */
        case 0: {
            size_t result = type_name.find("struct", 0);
            size_t offset = 0;
            if (result == 0) { offset = 6; } else {
                result = type_name.find("class", 0);
                if (result == 0) {
                    offset = 5;
                }
            }
            if (offset > 0) { type_name = type_name.substr(offset); }

            result = type_name.find("::", 0);
            if (result != std::string::npos) {
                offset = result + 2;
                type_name = type_name.substr(offset);
            }

            return type_name;
        }
                /* Namespace Included no 'class' or 'struct' keyword */
        case 1: {
            size_t result = type_name.find("struct", 0);
            size_t offset = 0;
            if (result != std::string::npos) { offset = 6; } else {
                result = type_name.find("class", 0);
                if (result != std::string::npos) { offset = 5; }
            }
            if (offset > 0) { type_name = type_name.substr(offset); }
            return type_name;
        }
                /* Normal typeid(T).name(); */
        case 2: break;
        }

        return type_name;
    }
}