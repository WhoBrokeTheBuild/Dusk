#ifndef DUSK_SHADER_MATERIAL_HPP
#define DUSK_SHADER_MATERIAL_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/Math.hpp>

namespace dusk {

struct DUSK_API ShaderMaterial
{
public:

    static constexpr uint32_t Binding = 2;

    static constexpr uint32_t BaseColorMapBinding = 3;
    static constexpr uint32_t NormalMapBinding = 4;
    static constexpr uint32_t MetallicRoughnessMapBinding = 5;
    static constexpr uint32_t EmissiveMapBinding = 6;
    static constexpr uint32_t OcclusionMapBinding = 7;

    alignas(4) vec4 BaseColorFactor;

    alignas(4) vec3 EmissiveFactor;

    alignas(4) float MetallicFactor;

    alignas(4) float RoughnessFactor;

    alignas(4) float OcclusionStrength;

    alignas(4) float NormalScale;

}; // struct ShaderMaterial

static_assert(
    sizeof(ShaderMaterial) == 44,
    "Layout of ShaderMaterial does not conform to std140"
);

} // namespace dusk

#endif // DUSK_SHADER_MATERIAL_HPP