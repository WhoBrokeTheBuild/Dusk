#ifndef DUSK_MESH_HPP
#define DUSK_MESH_HPP

#include <dusk/Config.hpp>

#include <dusk/core/Context.hpp>
#include <dusk/core/Util.hpp>
#include <dusk/asset/Shader.hpp>
#include <dusk/asset/Material.hpp>
#include <dusk/scene/IComponent.hpp>

using glm::mat4;
using glm::vec2;
using glm::vec3;

#include <string>
using std::string;

#include <utility>
using std::move;

#include <vector>
using std::vector;

#include <algorithm>
using std::swap;

#include <memory>
using std::unique_ptr;
using std::shared_ptr;

namespace dusk {

struct TransformData
{
    alignas(64) mat4 Model = mat4(1);
    alignas(64) mat4 View  = mat4(1);
    alignas(64) mat4 Proj  = mat4(1);
    alignas(64) mat4 MVP   = mat4(1);
};

class Mesh
{
public:

    struct Data
    {
        vector<vec3> Vertices;
        vector<vec3> Normals;
        vector<vec2> TexCoords;

        shared_ptr<dusk::Material> Material = nullptr;

        GLenum DrawMode = GL_TRIANGLES;
    };

    /// Class Boilerplate

    DISALLOW_COPY_AND_ASSIGN(Mesh)

    Mesh() = default;

    Mesh(Mesh&& rhs);

    Mesh(const string& filename);

    Mesh(const Data& data);

    Mesh(const vector<Data>& datum);

    virtual ~Mesh();

    bool LoadFromFile(const string& filename);

    string GetFilename() { return _filename; }

    void AddData(const Data& data);

    bool FinishLoad();

    bool IsLoaded() const { return _loaded; }

    Box GetBounds() const { return _bounds; }

    void Render(RenderContext& ctx);

private:

    struct RenderGroup
    {
        GLenum drawMode;
        shared_ptr<Material> material;
        size_t start;
        size_t count;
    };

    Box ComputeBounds(const vector<vec3>& verts);

    bool _loaded = false;
    string _filename;

    vector<Data> _data;

    GLuint _glVAO;
    GLuint _glVBOs[3];

    Box _bounds;

    vector<RenderGroup> _renderGroups;
};

} // namespace dusk

#endif // DUSK_MESH_HPP
