#ifndef DUSK_TRANSFORM_DATA_HPP
#define DUSK_TRANSFORM_DATA_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Math.hpp>

namespace Dusk {

struct DUSK_ENGINE_API TransformData
{
public:

    alignas(64) mat4 Model;
    alignas(64) mat4 View;
    alignas(64) mat4 Projection;
    alignas(64) mat4 MVP;

    inline void UpdateMVP() {
        MVP = Projection * View * Model;
    }

}; // struct TransformData

const unsigned TransformDataBinding = 0;

} // namespace Dusk

#endif // DUSK_TRANSFORM_DATA_HPP