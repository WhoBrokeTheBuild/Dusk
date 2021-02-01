#ifndef DUSK_SHADER_GLOBALS_HPP
#define DUSK_SHADER_GLOBALS_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Math.hpp>

namespace Dusk {

#define DUSK_SHADER_GLOBALS_BINDING 0

struct DUSK_ENGINE_API ShaderGlobals
{
public:

    alignas(8) vec2 Resolution;
    alignas(8) vec2 Mouse;
    alignas(4) int FrameCount;
    alignas(4) float TotalTime;
    alignas(4) float FrameSpeedRatio;

}; // struct ShaderGlobals

} // namespace Dusk

#endif // DUSK_SHADER_GLOBALS_HPP