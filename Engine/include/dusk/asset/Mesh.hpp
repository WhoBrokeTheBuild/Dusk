#ifndef DUSK_MESH_HPP
#define DUSK_MESH_HPP

#include <dusk/Config.hpp>
#include <dusk/asset/Material.hpp>
#include <dusk/asset/Shader.hpp>
#include <dusk/core/Context.hpp>
#include <dusk/core/Util.hpp>
#include <dusk/scene/ActorComponent.hpp>

#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace dusk {

class Mesh : public ActorComponent
{
public:

    enum AttributeID : GLint 
    {
        POSITION    = 0,
        NORMAL      = 1,
        TEXCOORD    = 2,
        TANGENT     = 3,
        BITANGENT   = 4,
    };

    struct Primitive 
    {
        GLuint      VAO;
        GLenum      Mode;
        GLsizei     Count;
        GLenum      Type;
        GLsizei     Offset;
        Box         Bounds;
        std::shared_ptr<Material> Material;
    };

    /// Class Boilerplate

    DISALLOW_COPY_AND_ASSIGN(Mesh)

    Mesh() = default;

    Mesh(std::vector<Primitive> primitives);

    Mesh(const std::string& filename);

    virtual ~Mesh();

    bool LoadFromFile(const std::string& filename);

    bool LoadFromData(std::vector<Primitive> primitives);

    std::string GetFilename() { 
        return _filename;
    }

    bool IsLoaded() const { 
        return _loaded; 
    }

    Box GetBounds() const { 
        return _bounds; 
    }

    virtual void Render(RenderContext& ctx) override;

    virtual void Print(std::string indent) override;

private:

    bool _loaded = false;

    std::string _filename;

    std::unique_ptr<Shader> _shader;

    std::vector<Primitive> _primitives;

    Box _bounds;
    
};

} // namespace dusk

#endif // DUSK_MESH_HPP
