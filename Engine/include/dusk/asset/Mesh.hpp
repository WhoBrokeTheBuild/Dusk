#ifndef DUSK_MESH_HPP
#define DUSK_MESH_HPP

#include <dusk/Config.hpp>
#include <dusk/core/Context.hpp>
#include <dusk/core/Util.hpp>
#include <dusk/asset/Shader.hpp>
#include <dusk/asset/Material.hpp>
#include <dusk/scene/IComponent.hpp>

#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <memory>

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

    enum AttributeID : GLint {
        POSITION    = 0,
        NORMAL      = 1,
        TEXCOORD    = 2,
        TANGENT     = 3,
        BITANGENT   = 4,
    };

    /// Class Boilerplate

    DISALLOW_COPY_AND_ASSIGN(Mesh)

    Mesh() = default;

    Mesh(Mesh&& rhs);

    Mesh(const string& filename);

    virtual ~Mesh();

    bool LoadFromFile(const string& filename);

    string GetFilename() { return _filename; }

    bool IsLoaded() const { return _loaded; }

    Box GetBounds() const { return _bounds; }

    void Render(RenderContext& ctx);

private:

    Box ComputeBounds(const vector<vec3>& verts);

    bool _loaded = false;

    string _filename;

    GLuint _glVAO;

    Box _bounds;

};

} // namespace dusk

#endif // DUSK_MESH_HPP
