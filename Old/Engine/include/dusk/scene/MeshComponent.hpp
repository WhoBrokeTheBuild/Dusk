#ifndef DUSK_MODEL_HPP
#define DUSK_MODEL_HPP

#include <dusk/asset/Mesh.hpp>
#include <dusk/core/Macros.hpp>
#include <dusk/scene/ActorComponent.hpp>

#include <memory>
#include <vector>

namespace dusk {

class MeshComponent : public ActorComponent
{
public:

    /// Class Boilerplate

    DISALLOW_COPY_AND_ASSIGN(MeshComponent)

    MeshComponent() = default;

    MeshComponent(std::shared_ptr<Mesh> mesh);

    virtual ~MeshComponent() = default;

    /// Methods

    virtual void AddMesh(std::shared_ptr<Mesh> mesh);
    
    virtual void Render(RenderContext& ctx) override;

    virtual void Print(std::string indent) override;

private:

    std::vector<std::shared_ptr<Mesh>> _meshes;

}; // class Model

} // namespace dusk

#endif // DUSK_MODEL_HPP
