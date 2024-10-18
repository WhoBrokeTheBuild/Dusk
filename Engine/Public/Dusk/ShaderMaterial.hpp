#ifndef DUSK_SHADER_MATERIAL_HPP
#define DUSK_SHADER_MATERIAL_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/Math.hpp>

namespace dusk {

struct DUSK_API ShaderMaterial
{
public:

    static constexpr uint32_t Binding = 2;

    alignas(4) vec4 BaseColorFactor;

    alignas(4) vec4 EmissiveFactor;

    alignas(4) float MetallicFactor;

    alignas(4) float RoughnessFactor;

    alignas(4) float OcclusionStrength;

    alignas(4) float NormalScale;

}; // struct ShaderMaterial

static_assert(
    sizeof(ShaderMaterial) == 48,
    "Layout of ShaderMaterial does not conform to std140"
);

} // namespace dusk

#endif // DUSK_SHADER_MATERIAL_HPP