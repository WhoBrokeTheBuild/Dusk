#ifndef DUSK_MESH_COMPONENT_HPP
#define DUSK_MESH_COMPONENT_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Component.hpp>
#include <Dusk/Mesh.hpp>

#include <memory>
#include <vector>

namespace Dusk {

class DUSK_ENGINE_API MeshComponent : public Component
{
public:

    DISALLOW_COPY_AND_ASSIGN(MeshComponent)

    MeshComponent() = default;

    virtual ~MeshComponent() = default;

    void SetMesh(std::shared_ptr<Mesh> mesh);

    void Render(RenderContext * ctx) override;

private:

    std::shared_ptr<Mesh> _mesh;

}; // class MeshComponent

} // namespace Dusk

#endif // DUSK_MESH_COMPONENT_HPP
