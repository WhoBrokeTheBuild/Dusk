#ifndef DUSK_SHADER_GLOBALS_HPP
#define DUSK_SHADER_GLOBALS_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/Math.hpp>

namespace dusk {

struct DUSK_API ShaderGlobals
{
public:

    static constexpr uint32_t Binding = 0;

    // Viewport resolution (in pixels)
    alignas(4) vec2 Resolution;

    // Mouse location (in pixels)
    alignas(4) vec2 Mouse;

    // Number of frames that have been rendered
    alignas(4) uint FrameCount;

    // Time passed since initialization (in seconds)
    alignas(4) float TotalTime;

    // Time passed since the previous frame (in seconds)
    alignas(4) float DeltaTime;

    // A ratio of DeltaTime / (1 / FPS)
    alignas(4) float FrameSpeedRatio;

}; // struct ShaderGlobals

static_assert(
    sizeof(ShaderGlobals) == 32,
    "Layout of ShaderGlobals does not conform to std140"
);

} // namespace dusk

#endif // DUSK_SHADER_GLOBALS_HPP