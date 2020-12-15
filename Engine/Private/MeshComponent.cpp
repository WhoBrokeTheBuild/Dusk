#include <Dusk/MeshComponent.hpp>

#include <Dusk/MeshImporter.hpp>
#include <Dusk/GraphicsDriver.hpp>
#include <Dusk/Entity.hpp>
#include <Dusk/Log.hpp>

namespace Dusk {

void MeshComponent::SetMesh(std::shared_ptr<Mesh> && mesh)
{
    // _meshes.push_back(std::move(mesh));
}

void MeshComponent::Render(RenderContext * ctx)
{
    // auto gfx = GetGraphicsDriver();
    // auto transformData = ctx->GetTransformData();

    // transformData->Model = GetEntity()->GetWorldTransform();
    // transformData->UpdateMVP();
    
    // gfx->SetShaderData("DuskTransformData", sizeof(TransformData), transformData);

    // _mesh->Render();
}

} // namespace Dusk
