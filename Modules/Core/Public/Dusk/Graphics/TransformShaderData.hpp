#ifndef DUSK_TRANSFORM_SHADER_DATA_HPP
#define DUSK_TRANSFORM_SHADER_DATA_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Math.hpp>

namespace Dusk {

struct DUSK_CORE_API TransformShaderData
{
    mat4 Model;
    mat4 View;
    mat4 Projection;
};

} // namespace Dusk

#endif // DUSK_TRANSFORM_SHADER_DATA_HPP