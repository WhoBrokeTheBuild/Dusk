#ifndef DUSK_MODEL_HPP
#define DUSK_MODEL_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/Path.hpp>
#include <Dusk/Mesh.hpp>
#include <Dusk/Transform.hpp>
#include <Dusk/ShaderTransform.hpp>

namespace dusk {

class DUSK_API Model
{
public:

    Model() = default;

    Transform Transform;

    bool LoadFromFile(const Path& path);

    void Create(List<Mesh::Pointer>&& meshList);

    void Free();

    bool CanReload() {
        return not _path.empty();
    }

    bool Reload() {
        return LoadFromFile(_path);
    }

    // TODO: Move and Replace
    void Update(mat4 view, mat4 proj);

    inline BoundingBox GetBounds() const {
        BoundingBox bounds;
        for (const auto& mesh : _meshList) {
            bounds.Extend(mesh->GetBounds());
        }
        return bounds;
    }

    inline void GenerateCommands(VkCommandBuffer commandBuffer) {
        for (auto& mesh : _meshList) {
            mesh->GenerateCommands(commandBuffer);
        }
    }

private:

    Path _path;

    List<Mesh::Pointer> _meshList;

    VulkanBuffer _transformBuffer;

}; // class Model

} // namespace

#endif // DUSK_MODEL_HPP