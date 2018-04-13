#ifndef DUSK_MESH_HPP
#define DUSK_MESH_HPP

#include <dusk/Config.hpp>

#include <dusk/core/Context.hpp>
#include <dusk/core/Util.hpp>
#include <dusk/asset/Shader.hpp>
#include <dusk/asset/Material.hpp>
#include <memory>
#include <sstream>

namespace dusk {

class Mesh;

class Mesh
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

    static std::shared_ptr<Mesh> Create();
    static std::shared_ptr<Mesh> Create(const std::string& filename);
    static std::shared_ptr<Mesh> Create(const Data& data);
    static std::shared_ptr<Mesh> Create(const std::vector<Data>& datum);

    Mesh() = default;

    Mesh(Mesh&& rhs)
    {
        std::swap(_loaded, rhs._loaded);
        std::swap(_glVAO, rhs._glVAO);
        memcpy(_glVBOs, rhs._glVBOs, sizeof(_glVBOs));
        memset(rhs._glVBOs, 0, sizeof(rhs._glVBOs));
        _renderGroups = std::move(rhs._renderGroups);
    }

    Mesh(const std::string& filename)
    {
        LoadFromFile(filename);
    }

    Mesh(const Data& data)
    {
        AddData(data);
        FinishLoad();
    }

    Mesh(const std::vector<Data>& datum)
    {
        for (const auto& data : datum)
        {
            AddData(data);
        }
        FinishLoad();
    }

    virtual ~Mesh();

    void Serialize(nlohmann::json& data);
    void Deserialize(nlohmann::json& data);

    bool LoadFromFile(const std::string& filename);

    std::string GetFilename() { return _filename; }

    void AddData(const Data& data);

    bool FinishLoad();

    bool IsLoaded() const { return _loaded; }

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

    Box ComputeBounds(const std::vector<glm::vec3>& verts);

    bool _loaded = false;
    std::string _filename;

    std::vector<Data> _data;

    GLuint _glVAO;
    GLuint _glVBOs[3];

    Box _bounds;

    std::vector<RenderGroup> _renderGroups;
};

} // namespace dusk

#endif // DUSK_MESH_HPP
