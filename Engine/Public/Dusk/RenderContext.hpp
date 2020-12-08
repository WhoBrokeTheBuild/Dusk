#ifndef DUSK_RENDER_CONTEXT_HPP
#define DUSK_RENDER_CONTEXT_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/TransformData.hpp>

namespace Dusk {

class DUSK_ENGINE_API RenderContext : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(RenderContext)

    RenderContext() = default;

    virtual ~RenderContext() = default;

    inline std::string GetClassID() const override {
        return "Dusk::UpdateContext";
    }

    virtual TransformData * GetTransformData() {
        return &_transformData;
    }

private:

    TransformData _transformData;

}; // class RenderContext

} // namespace Dusk

#endif // DUSK_RENDER_CONTEXT_HPP