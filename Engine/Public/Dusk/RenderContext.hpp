#ifndef DUSK_RENDER_CONTEXT_HPP
#define DUSK_RENDER_CONTEXT_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Graphics/TransformData.hpp>

namespace Dusk {

class RenderContext
{
public:

    DISALLOW_COPY_AND_ASSIGN(RenderContext)

    RenderContext() = default;

    virtual ~RenderContext() = default;

    virtual TransformData * GetTransformData() {
        return &_transformData;
    }

private:

    TransformData _transformData;

}; // class RenderContext

} // namespace Dusk

#endif // DUSK_RENDER_CONTEXT_HPP