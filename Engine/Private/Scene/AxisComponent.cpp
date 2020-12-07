#include <Dusk/Scene/AxisComponent.hpp>
#include <Dusk/Graphics/GraphicsDriver.hpp>
#include <Dusk/Scene/Entity.hpp>

namespace Dusk {

DUSK_ENGINE_API
AxisMeshData AxisComponent::_MeshData;

DUSK_ENGINE_API
AxisComponent::AxisComponent()
{
    auto gfx = GetGraphicsDriver();

    _shader = gfx->CreateShader();
    _shader->LoadFromFiles({
        "Dusk/axis.vert",
        "Dusk/axis.frag",
    });

    _mesh = gfx->CreateMesh();
    _mesh->Load(&_MeshData);
    // TODO: Make GL_LINES
}

DUSK_ENGINE_API
void AxisComponent::Render(RenderContext * ctx)
{
    auto gfx = GetGraphicsDriver();
    auto transformData = ctx->GetTransformData();

    transformData->Model = GetEntity()->GetWorldTransform();
    transformData->UpdateMVP();
    
    gfx->SetShaderData("DuskTransformData", sizeof(TransformData), transformData);

    // _shader->Bind();
    _mesh->Render();
}

} // namespace Dusk