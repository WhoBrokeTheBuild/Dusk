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

class Mesh
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

    /// Class Boilerplate

    DISALLOW_COPY_AND_ASSIGN(Mesh)

    Mesh() = default;

    Mesh(Mesh&& rhs);

    Mesh(const std::string& filename);

    virtual ~Mesh();

    bool LoadFromFile(const std::string& filename);

    std::string GetFilename() { 
        return _filename; 
    }

    bool IsLoaded() const { 
        return _loaded; 
    }

	Box GetBounds() const { 
		return _bounds; 
	}

    void Render(RenderContext& ctx);

private:

    bool _loaded = false;

    std::string _filename;

	Box _bounds;

    GLuint _glVAO;

	struct Primitive 
    {
		GLenum	mode;
		GLsizei count;
		GLenum	type;
		GLsizei offset;
        int material;
	};

    std::vector<std::unique_ptr<Material>> _materials;

	std::vector<Primitive> _primitives;

	void ComputeBounds(const glm::vec3* data, size_t length);
	
};

} // namespace dusk

#endif // DUSK_MESH_HPP
