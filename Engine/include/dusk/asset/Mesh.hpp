#ifndef DUSK_MESH_HPP
#define DUSK_MESH_HPP

#include <dusk/asset/ILoadable.hpp>
#include <dusk/asset/Material.hpp>
#include <dusk/core/Context.hpp>
#include <dusk/core/Macros.hpp>
#include <dusk/core/Math.hpp>
#include <dusk/core/Shader.hpp>
#include <dusk/core/Util.hpp>

#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace dusk {

class Mesh : public ILoadable
{
public:

    enum AttributeID : GLint 
    {
        POSITION    = 0,
        NORMAL      = 1,
        UV          = 2,
        COLOR       = 3,
        TANGENT     = 4,
        BITANGENT   = 5,
    };

    struct Primitive 
    {
        GLuint      VAO;
        GLenum      Mode;
        GLsizei     Count;
        GLenum      Type;
        GLsizei     Offset;
        Box         Bounds;
        
        std::shared_ptr<dusk::Material> Material;
        std::unique_ptr<dusk::Shader> Shader;
    };

    /// Class Boilerplate

    DISALLOW_COPY_AND_ASSIGN(Mesh)

    Mesh() = default;

    Mesh(Primitive p);

    Mesh(std::vector<Primitive>&&);

    Mesh(const std::string&);

    virtual ~Mesh();

    bool LoadFromFile(const std::string&);

    bool LoadFromData(std::vector<Primitive>&&);

    std::string GetFilename() const { 
        return _filename;
    }

    Box GetBounds() const { 
        return _bounds; 
    }

    std::vector<Primitive>& GetPrimitives() {
        return _primitives;
    }

    virtual void Render(RenderContext&, glm::mat4 transform = glm::mat4(1.f));

private:

    std::string _filename;

    std::vector<Primitive> _primitives;

    Box _bounds;
    
};

} // namespace dusk

#endif // DUSK_MESH_HPP
