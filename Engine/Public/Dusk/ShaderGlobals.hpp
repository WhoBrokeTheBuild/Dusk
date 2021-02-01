#ifndef DUSK_SHADER_GLOBALS_HPP
#define DUSK_SHADER_GLOBALS_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Math.hpp>

namespace Dusk {

#define DUSK_SHADER_GLOBALS_BINDING 0

struct DUSK_ENGINE_API ShaderGlobals
{
public:

    alignas(8) ivec2 Resolution;
    alignas(8) ivec2 Mouse;
    alignas(8) uint64_t FrameCount;
    // alignas(8) int64_t TotalTime;
    alignas(4) float FrameSpeedRatio;

}; // struct ShaderGlobals

} // namespace Dusk

#endif // DUSK_SHADER_GLOBALS_HPP