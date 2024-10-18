#ifndef DUSK_SHADER_TRANSFORM_HPP
#define DUSK_SHADER_TRANSFORM_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/Math.hpp>

namespace dusk {

struct DUSK_API ShaderTransform
{
public:

    static constexpr uint32_t Binding = 1;

    alignas(4) mat4 Model;
    
    alignas(4) mat4 View;
    
    alignas(4) mat4 Projection;
    
    alignas(4) mat4 MVP;

    inline void UpdateMVP() {
        MVP = Projection * View * Model;
    }

}; // struct ShaderTransform

static_assert(
    sizeof(ShaderTransform) == 256,
    "Layout of ShaderTransform does not conform to std140"
);

} // namespace dusk

#endif // DUSK_SHADER_TRANSFORM_HPP