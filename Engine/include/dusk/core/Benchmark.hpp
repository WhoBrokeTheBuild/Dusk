#ifndef DUSK_BENCHMARK_HPP
#define DUSK_BENCHMARK_HPP

#include <dusk/Config.hpp>
#include <dusk/core/Log.hpp>

#include <chrono>
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;

namespace dusk {

#define DuskBenchStart() \
    auto duskBenchClockStart = high_resolution_clock::now();

#define DuskBenchEnd(funcName)                                  \
    DuskLogPerf("Function: %s took %.3f millis", funcName,      \
        duration_cast<duration<double, std::milli>>(            \
            high_resolution_clock::now() - duskBenchClockStart  \
        ).count());

} // namespace

#endif // DUSK_BENCHMARK_HPP
