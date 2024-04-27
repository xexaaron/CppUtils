#include "../Public/EnumTest.h"
#include <fstream>
#define SIZE_OF_ETEST_2 10

using namespace Utility;

int main() {
    
#ifdef _WIN32
    NullStream.open("nul");
#else 
    NullStream.open("/dev/null");
#endif // _WIN32
    
    if (!NullStream.is_open()) {
        std::cerr << "Failed to open null stream file." << std::endl;
        return 1;
    }

    size_t Iterations = 100;


    TIME_SCOPE(
        for (int rep = 0; rep < Iterations; ++rep) {
            ETest::Iterate(&PrintETest);
        }
    )

    auto avgAdvancedEnumTime = elapsed.count() / Iterations;

    TIME_SCOPE(
        for (int rep = 0; rep < Iterations; ++rep) {
            for (uint8_t i = 0; i < SIZE_OF_ETEST_2; i++) {
                PrintETest2(static_cast<ETest2>(i));
            }
        }
    )

    auto avgEnumSwitchTime = elapsed.count() / Iterations;

    std::cout << "Iterations                      : "  << Iterations                   << std::endl;
    std::cout << "(ms) Avg Elapsed Enum (Array/Vector) : " << (float)avgAdvancedEnumTime / 1000.f << std::endl; // milliseconds
    std::cout << "(ns) Avg Elapsed Enum (Array/Vector) : " << avgAdvancedEnumTime << std::endl; // nanoseconds

    std::cout << "(ms) Avg Elapsed Enum (Switch)       : " << (float)avgEnumSwitchTime / (float)1000 << std::endl; // milliseconds
    std::cout << "(ns) Avg Elapsed Enum (Switch)       : " << avgEnumSwitchTime << std::endl; // nanoseconds


    NullStream.close();

    TestBitwiseOperations();

    return 0;
}