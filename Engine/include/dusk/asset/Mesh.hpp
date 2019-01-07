#ifndef DUSK_MESH_HPP
#define DUSK_MESH_HPP

#include <dusk/Config.hpp>
#include <dusk/asset/Material.hpp>
#include <dusk/asset/Shader.hpp>
#include <dusk/core/Context.hpp>
#include <dusk/core/Math.hpp>
#include <dusk/core/Util.hpp>
#include <dusk/scene/ActorComponent.hpp>

#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace dusk {

class Mesh
{
public:

    enum AttributeID : GLint 
    {
        POSITION    = 0,
        NORMAL      = 1,
        TEXCOORD    = 2,
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
        
        std::shared_ptr<Material> Mat;
    };

    /// Class Boilerplate

    DISALLOW_COPY_AND_ASSIGN(Mesh)

    Mesh() = default;

    Mesh(Primitive p, std::unique_ptr<Shader>&& shader = nullptr);

    Mesh(std::vector<Primitive> primitives, std::unique_ptr<Shader>&& shader = nullptr);

    Mesh(const std::string& filename, std::unique_ptr<Shader>&& shader = nullptr);

    virtual ~Mesh();

    bool LoadFromFile(const std::string& filename, std::unique_ptr<Shader>&& shader = nullptr);

    bool LoadFromData(std::vector<Primitive> primitives, std::unique_ptr<Shader>&& shader = nullptr);

    std::string GetFilename() const { 
        return _filename;
    }

    void SetShader(std::unique_ptr<Shader>&& shader);

    Shader * GetShader() const {
        return _shader.get();
    }

    bool IsLoaded() const { 
        return _loaded; 
    }

    Box GetBounds() const { 
        return _bounds; 
    }

    virtual void Render(RenderContext& ctx, glm::mat4 transform = glm::mat4(1.f));

private:

    bool _loaded = false;

    std::string _filename;

    std::unique_ptr<Shader> _shader;

    std::vector<Primitive> _primitives;

    Box _bounds;
    
};

} // namespace dusk

#endif // DUSK_MESH_HPP
