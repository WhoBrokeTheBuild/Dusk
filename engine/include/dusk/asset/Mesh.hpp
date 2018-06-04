#ifndef DUSK_MESH_HPP
#define DUSK_MESH_HPP

#include <dusk/Config.hpp>

#include <dusk/core/Context.hpp>
#include <dusk/core/Util.hpp>
#include <dusk/asset/Asset.hpp>
#include <dusk/asset/Shader.hpp>
#include <dusk/asset/Material.hpp>
#include <dusk/scene/IComponent.hpp>

#include <memory>
#include <sstream>

namespace dusk {

struct TransformData
{
    alignas(64) glm::mat4 Model = glm::mat4(1);
    alignas(64) glm::mat4 View  = glm::mat4(1);
    alignas(64) glm::mat4 Proj  = glm::mat4(1);
    alignas(64) glm::mat4 MVP   = glm::mat4(1);
};

class Mesh : public Asset
{
public:

    struct Data
    {
        std::vector<glm::vec3> Vertices;
        std::vector<glm::vec3> Normals;
        std::vector<glm::vec2> TexCoords;

        std::shared_ptr<dusk::Material> Material = nullptr;

        GLenum DrawMode = GL_TRIANGLES;
    };

    /// Class Boilerplate

    DISALLOW_COPY_AND_ASSIGN(Mesh)

    Mesh() = default;

    Mesh(const std::string& filename);

    virtual ~Mesh();

    Box GetBounds() const { return _bounds; }

    void Render(RenderContext& ctx);

private:

    struct RenderGroup
    {
        GLenum drawMode;
        std::shared_ptr<Material> material;
        size_t start;
        size_t count;
    };

    virtual void Load() override;
    virtual void Free() override;

    Box ComputeBounds(const std::vector<glm::vec3>& verts);

    std::vector<Data> _data;

    GLuint _glVAO;
    GLuint _glVBOs[3];

    Box _bounds;

    std::vector<RenderGroup> _renderGroups;
};

class MeshComponent : public IComponent
{
public:

    MeshComponent(Actor * actor, Mesh * mesh = nullptr);
    virtual ~MeshComponent();

    virtual void Serialize(nlohmann::json& data);
    virtual void Deserialize(nlohmann::json& data);

    void Render(RenderContext& ctx);

    Mesh * GetMesh() const { return _mesh; }

    void SetMesh(Mesh * mesh);

private:

    TransformData _shaderData;

    Mesh * _mesh;

}; // class MeshComponent

} // namespace dusk

#endif // DUSK_MESH_HPP
