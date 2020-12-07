#ifndef DUSK_OPENGL_MESH_HPP
#define DUSK_OPENGL_MESH_HPP

#include <Dusk/OpenGL/Config.hpp>
#include <Dusk/Graphics/Mesh.hpp>

namespace Dusk {

class DUSK_OPENGL_API OpenGLMesh : public Mesh 
{
public:

    DISALLOW_COPY_AND_ASSIGN(OpenGLMesh)

    OpenGLMesh() = default;

    virtual ~OpenGLMesh() = default;

    inline std::string GetClassID() const override {
        return "Dusk::OpenGLMesh";
    }

    void Render() override;

    bool Load(const MeshData * data) override;

private:

    GLenum GetGLMode(const MeshData::Mode& mode);

    GLuint _glVAO;
    GLenum _glMode;
    GLsizei _glCount;

    bool _indexed;

}; // class OpenGLMesh

} // namespace Dusk

#endif // DUSK_OPENGL_MESH_HPP
