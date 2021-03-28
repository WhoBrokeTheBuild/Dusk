#include <Dusk/AxisComponent.hpp>

#include <Dusk/GraphicsDriver.hpp>
#include <Dusk/Entity.hpp>

namespace Dusk {

// DUSK_CORE_API
// AxisPrimitiveData AxisComponent::_PrimitiveData;

DUSK_CORE_API
AxisComponent::AxisComponent()
{
    auto gfx = GraphicsDriver::GetInstance();
    assert(gfx);

    _shader = gfx->CreateShader();
    _shader->LoadFromFiles({
        "Dusk/axis.vert",
        "Dusk/axis.frag",
    });

    _mesh = gfx->CreateMesh();
    // _mesh->Load(&_PrimitiveData);
    // TODO: Make GL_LINES
}

DUSK_CORE_API
void AxisComponent::Render(RenderContext * ctx)
{
    // auto gfx = GraphicsDriver::GetInstance();
    auto transformData = ctx->GetShaderTransform();

    transformData->Model = GetEntity()->GetWorldTransform();
    transformData->UpdateMVP();
    
    // gfx->SetShaderData("DuskTransformData", sizeof(TransformData), transformData);

    // _shader->Bind();
    // _mesh->Render();
}

} // namespace Dusk