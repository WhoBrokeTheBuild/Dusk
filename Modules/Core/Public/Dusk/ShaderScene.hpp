#ifndef DUSK_SHADER_SCENE_HPP
#define DUSK_SHADER_SCENE_HPP

#include <Dusk/Math.hpp>

namespace Dusk {

#define DUSK_MAX_LIGHT_COUNT 10

struct LightData
{
    alignas(16) vec3 Position;
    alignas(4)  uint8_t Type;
    alignas(16) vec3 Direction;
    alignas(4)  float InnerCutOff;
    alignas(16) vec3 Color;
    alignas(4)  float OuterCutOff;

}; // struct LightData

struct ShaderScene 
{
    vec3 CameraPosition;

    vec3 AmbientLightColor;
    vec3 AmbientLightDirection;

    LightData Lights[DUSK_MAX_LIGHT_COUNT];

}; // struct ShaderScene 

} // namespace Dusk

#endif // DUSK_SHADER_SCENE_HPP