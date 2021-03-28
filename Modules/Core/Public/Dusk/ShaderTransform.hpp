#ifndef DUSK_SHADER_TRANSFORM_HPP
#define DUSK_SHADER_TRANSFORM_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Math.hpp>

namespace Dusk {

#define DUSK_SHADER_TRANSFORM_BINDING 1

struct DUSK_CORE_API ShaderTransform
{
public:

    alignas(4) mat4 Model;
    
    alignas(4) mat4 View;
    
    alignas(4) mat4 Projection;
    
    alignas(4) mat4 MVP;

    inline void UpdateMVP() {
        MVP = Projection * View * Model;
    }

}; // struct ShaderTransform

} // namespace Dusk

#endif // DUSK_SHADER_TRANSFORM_HPP