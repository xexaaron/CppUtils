#pragma once

#include <iostream>
#include <memory>
#include <any>

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

    /* Tempalte specializations for primitive types */
    template <> std::string GetTypename<void>(unsigned int verbosity) { return "void"; }
    template <> std::string GetTypename<int>(unsigned int verbosity) { return "int"; }
    template <> std::string GetTypename<unsigned int>(unsigned int verbosity) { return "unsigned int"; }
    template <> std::string GetTypename<float>(unsigned int verbosity) { return "float"; }
    template <> std::string GetTypename<double>(unsigned int verbosity) { return "double"; }
    template <> std::string GetTypename<long double>(unsigned int verbosity) { return "long double"; }
    template <> std::string GetTypename<char>(unsigned int verbosity) { return "char"; }
    template <> std::string GetTypename<unsigned char>(unsigned int verbosity) { return "unsigned char"; }
    template <> std::string GetTypename<bool>(unsigned int verbosity) {  return "bool"; }
    template <> std::string GetTypename<short>(unsigned int verbosity) { return "short"; }
    template <> std::string GetTypename<unsigned short>(unsigned int verbosity) { return "unsigned short"; }
    template <> std::string GetTypename<long>(unsigned int verbosity) { return "long"; }
    template <> std::string GetTypename<unsigned long>(unsigned int verbosity) { return "unsigned long"; }
    template <> std::string GetTypename<long long>(unsigned int verbosity) { return "long long"; }
    template <> std::string GetTypename<unsigned long long>(unsigned int verbosity) { return "unsigned long long"; }
    template <> std::string GetTypename<wchar_t>(unsigned int verbosity) { return "wchar_t"; }

    /* Template sepcializations for verbose aliases or other */
    template <> std::string GetTypename<std::string>(unsigned int verbosity) { return "std::string"; }
    template <> std::string GetTypename<char16_t>(unsigned int verbosity) { return "char16_t"; }
    template <> std::string GetTypename<char32_t>(unsigned int verbosity) { return "char32_t"; }
    template <> std::string GetTypename<std::byte>(unsigned int verbosity) { return "std::byte"; }

}

