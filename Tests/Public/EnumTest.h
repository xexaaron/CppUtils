#pragma once

#include "../../Enum/Enum.h"
#include <iostream>
#include <chrono>
#include <fstream>
#include <cstdlib>

AdvancedEnum(ETest, uint8_t,
    VAL_ONE,
    VAL_TWO,
    VAL_THREE,
    VAL_FOUR,
    VAL_FIVE,
    VAL_SIX,
    VAL_SEVEN,
    VAL_EIGHT,
    VAL_NINE,
    VAL_TEN
);


enum class ETest2 : uint8_t {
    VAL_ONE,
    VAL_TWO,
    VAL_THREE,
    VAL_FOUR,
    VAL_FIVE,
    VAL_SIX,
    VAL_SEVEN,
    VAL_EIGHT,
    VAL_NINE,
    VAL_TEN
};

static std::ofstream NullStream;

static void PrintETest2(ETest2 var) {
    switch (var) {
        case ETest2::VAL_ONE:
            NullStream << "VAL_ONE" << std::endl;
            break;
        case ETest2::VAL_TWO:
            NullStream << "VAL_TWO" << std::endl;
            break;
        case ETest2::VAL_THREE:
            NullStream << "VAL_THREE" << std::endl;
            break;
        case ETest2::VAL_FOUR:
            NullStream << "VAL_FOUR" << std::endl;
            break;
        case ETest2::VAL_FIVE:
            NullStream << "VAL_FIVE" << std::endl;
            break;
        case ETest2::VAL_SIX:
            NullStream << "VAL_SIX" << std::endl;
            break;
        case ETest2::VAL_SEVEN:
            NullStream << "VAL_SEVEN" << std::endl;
            break;
        case ETest2::VAL_EIGHT:
            NullStream << "VAL_EIGHT" << std::endl;
            break;
        case ETest2::VAL_NINE:
            NullStream << "VAL_NINE" << std::endl;
            break;
        case ETest2::VAL_TEN:
            NullStream << "VAL_TEN" << std::endl;
            break;
    }
}

static void PrintETest(ETest var) {
    NullStream << var << std::endl;
}

static std::chrono::nanoseconds elapsed;

#define TIME_SCOPE(...) { \
        auto start = std::chrono::high_resolution_clock::now(); \
        __VA_ARGS__ \
        auto end = std::chrono::high_resolution_clock::now(); \
        elapsed = end - start; \
    }

void TestBitwiseOperations() {
    ETest value1 = ETest::VAL_ONE; 
    ETest value2 = ETest::VAL_TWO;

    // Perform bitwise AND
    ETest andResult = value1 & value2;
    std::cout << "Bitwise AND: " << value1.String() << " & " << value2.String() << " = " << andResult.String() << std::endl;

    // Perform bitwise OR
    ETest orResult = value1 | value2;
    std::cout << "Bitwise OR: " << value1.String() << " | " << value2.String() << " = " << orResult.String() << std::endl;

    // Perform bitwise XOR
    ETest xorResult = value1 ^ value2;
    std::cout << "Bitwise XOR: " << value1.String() << " ^ " << value2.String() << " = " << xorResult.String() << std::endl;

    // Perform bitwise NOT
    ETest notResult = ~value1;
    std::cout << "Bitwise NOT: ~" << value1.String() << " = " << notResult.String() << std::endl;
}

void CompareTestResult(long long avgAdvancedEnumTime, long long avgEnumSwitchTime) {
    const char* out = avgAdvancedEnumTime > avgEnumSwitchTime ? "Basic enum switch case printing" : "Advanced enum iteration printing";
    long long diff = avgAdvancedEnumTime > avgEnumSwitchTime ? avgAdvancedEnumTime - avgEnumSwitchTime : avgEnumSwitchTime - avgAdvancedEnumTime;
    float factor = avgAdvancedEnumTime > avgEnumSwitchTime ? (float)avgAdvancedEnumTime / (float)avgEnumSwitchTime : (float)avgEnumSwitchTime / (float)avgAdvancedEnumTime;
    std::cout << "Faster     : " << out << std::endl;
    std::cout << "Difference : " << diff << "ns" << std::endl;
    printf("Factor     : %.2f times slower\n", factor);  
}