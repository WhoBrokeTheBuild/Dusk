#ifndef DUSK_RENDER_CONTEXT_HPP
#define DUSK_RENDER_CONTEXT_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/ShaderGlobals.hpp>
#include <Dusk/ShaderTransform.hpp>

namespace Dusk {

class DUSK_CORE_API RenderContext : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(RenderContext)

    RenderContext() = default;

    virtual ~RenderContext() = default;

    virtual ShaderGlobals * GetShaderGlobals() {
        return &_shaderGlobals;
    }

    virtual ShaderTransform * GetShaderTransform() {
        return &_shaderTransform;
    }

private:

    ShaderGlobals _shaderGlobals;

    ShaderTransform _shaderTransform;

}; // class RenderContext

} // namespace Dusk

#endif // DUSK_RENDER_CONTEXT_HPP