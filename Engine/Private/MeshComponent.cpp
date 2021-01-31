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
    auto gfx = GetGraphicsDriver();
    TransformData * transformData = ctx->GetTransformData();

    transformData->Model = GetEntity()->GetWorldTransform();
    transformData->UpdateMVP();

    Buffer * buffer = _mesh->GetTransformDataBuffer();
    if (buffer) {
        buffer->WriteTo(0, sizeof(TransformData), reinterpret_cast<uint8_t *>(ctx->GetTransformData()));
    }

    _mesh->Render(ctx);    
}

} // namespace Dusk
