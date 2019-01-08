#ifndef DUSK_MODEL_HPP
#define DUSK_MODEL_HPP

#include <dusk/Config.hpp>
#include <dusk/asset/Mesh.hpp>
#include <dusk/scene/ActorComponent.hpp>

#include <memory>
#include <vector>

namespace dusk {

class MeshComponent : public ActorComponent
{
public:

    MeshComponent() = default;

    virtual ~MeshComponent() = default;

    virtual void AddMesh(std::shared_ptr<Mesh> mesh);
    
    virtual void Render(RenderContext& ctx) override;

    virtual void Print(std::string indent) override;

private:

    std::vector<std::shared_ptr<Mesh>> _meshes;

}; // class Model

} // namespace dusk

#endif // DUSK_MODEL_HPP
