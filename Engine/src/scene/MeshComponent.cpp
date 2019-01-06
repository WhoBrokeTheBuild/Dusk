#include "dusk/scene/MeshComponent.hpp"

#include <dusk/scene/Actor.hpp>

namespace dusk {

MeshComponent::MeshComponent(Actor * actor) 
    : ActorComponent(actor)
{ }

void MeshComponent::AddMesh(std::shared_ptr<Mesh> mesh)
{
    _meshes.push_back(std::move(mesh));
}

void MeshComponent::Render(RenderContext& ctx) 
{
    if (!GetActor()) {
        return;
    }

    glm::mat4 model = GetActor()->GetWorldTransform();
    for (auto& mesh : _meshes) {
        mesh->Render(ctx, model);
    }
}

void MeshComponent::Print(std::string indent)
{
    ActorComponent::Print(indent);
    DuskLog("%s _meshes.size = %zu", indent, _meshes.size());
}

}
