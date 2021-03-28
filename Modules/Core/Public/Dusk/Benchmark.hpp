#ifndef DUSK_BENCHMARK_HPP
#define DUSK_BENCHMARK_HPP

#include <Dusk/Log.hpp>

#include <chrono>

namespace Dusk {

#define DuskBenchmarkStart() \
    auto duskBenchClockStart = std::chrono::high_resolution_clock::now();

#define DuskBenchmarkEnd()                                                      \
    Dusk::LogPerformance(DUSK_ANCHOR,                                           \
    "Function: {} took {:.3} millis", DUSK_FUNCTION_NAME(),                     \
        std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(  \
            std::chrono::high_resolution_clock::now() - duskBenchClockStart     \
        ).count());

} // namespace Dusk

#endif // DUSK_BENCHMARK_HPP
