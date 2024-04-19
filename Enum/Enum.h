#pragma once
#include "EnumImpl.h"
/**
 * @brief Advanced enum structure. (comments contain additional info that some IDE's might not pick up)
 *
 * @attention Enums have a null value ENULL which is defined as the maximum possible value based on the underlying enum type.
 * @attention Do not use the internal enum to iterate always use the AdvancedEnum struct
 * 
 * @param Length() Number of enum elements. (including ENULL).
 * @param ToString(enum) Static method for string conversion.
 * @param String() Object method for string conversion.
 * @param Iterate(void(enum)()) Iterate over all enum elements. eg. @code Iterate([](enum val){ std::cout << val << std::endl; }); @endcode.
 * @param Name() String representation of the enum name.
 * @param IsNull() Checks whether the enum is ENULL or not.
 * @param Max() Returns the maximum underlying type value possible for this enum (minus one, max is reserved for ENULL).
 * @param << Printable to the output stream.
 *
 * @return Implicit conversion to underlying enum.
 *
 * @note String conversion requires the creation of an array.
 * @note Size and alignment of the enum struct are equivalent to that of the underlying enum type.
 * @note Enums have underlying types eg. uint8_t, uint16_t.
 * @note Enum highlighting when defining an AdvancedEnum does not work.
 */
#define AdvancedEnum(eName, eType, ...) CREATE_ENUM(eName, eType, __VA_ARGS__)
