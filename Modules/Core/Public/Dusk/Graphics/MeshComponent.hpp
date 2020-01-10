#ifndef DUSK_MESH_COMPONENT_HPP
#define DUSK_MESH_COMPONENT_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Macros.hpp>
#include <Dusk/Scene/Component.hpp>
#include <Dusk/Graphics/Mesh.hpp>

namespace Dusk {

class DUSK_CORE_API MeshComponent : public IComponent
{
public:

    DISALLOW_COPY_AND_ASSIGN(MeshComponent)

    explicit MeshComponent() = default;

    explicit MeshComponent(std::unique_ptr<IMesh> && mesh);

    explicit MeshComponent(std::vector<std::unique_ptr<IMesh>> && meshes);

    virtual ~MeshComponent() = default;

    bool LoadFromFile(const std::string& filename);

    void AddMesh(std::unique_ptr<IMesh> && mesh);

    void AddMeshes(std::vector<std::unique_ptr<IMesh>> && meshes);

    void Render(RenderContext * ctx) override;

private:

    std::vector<std::unique_ptr<IMesh>> _meshes;

}; // class MeshComponent

} // namespace Dusk

#endif // DUSK_MESH_COMPONENT_HPP
