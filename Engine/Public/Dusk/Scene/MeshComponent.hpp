#ifndef DUSK_MESH_COMPONENT_HPP
#define DUSK_MESH_COMPONENT_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Scene/Component.hpp>
#include <Dusk/Graphics/Mesh.hpp>

#include <memory>
#include <vector>

namespace Dusk {

class DUSK_ENGINE_API MeshComponent : public Component
{
public:

    DISALLOW_COPY_AND_ASSIGN(MeshComponent)

    explicit MeshComponent() = default;

    explicit MeshComponent(std::unique_ptr<Mesh> && mesh);

    explicit MeshComponent(std::vector<std::unique_ptr<Mesh>> && meshes);

    virtual ~MeshComponent() = default;

    bool LoadFromFile(const std::string& filename);

    void AddMesh(std::unique_ptr<Mesh> && mesh);

    void AddMeshes(std::vector<std::unique_ptr<Mesh>> && meshes);

    void Render(RenderContext * ctx) override;

private:

    std::vector<std::unique_ptr<Mesh>> _meshes;

}; // class MeshComponent

} // namespace Dusk

#endif // DUSK_MESH_COMPONENT_HPP
