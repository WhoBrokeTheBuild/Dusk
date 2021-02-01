#include <Dusk/MeshComponent.hpp>

#include <Dusk/MeshImporter.hpp>
#include <Dusk/GraphicsDriver.hpp>
#include <Dusk/Entity.hpp>
#include <Dusk/Log.hpp>

namespace Dusk {

void MeshComponent::SetMesh(std::shared_ptr<Mesh> mesh)
{
    _mesh = mesh;
}

void MeshComponent::Render(RenderContext * ctx)
{
    auto transform = ctx->GetShaderTransform();

    transform->Model = GetEntity()->GetWorldTransform();
    transform->UpdateMVP();

    _mesh->Render(ctx);
}

} // namespace Dusk
