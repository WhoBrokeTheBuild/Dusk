#ifndef DUSK_OPENGL_MESH_HPP
#define DUSK_OPENGL_MESH_HPP

#include <Dusk/OpenGL/OpenGLConfig.hpp>

#include <Dusk/Mesh.hpp>

namespace Dusk::OpenGL {

#define DUSK_OPENGL_MESH(x) (dynamic_cast<Dusk::OpenGL::OpenGLMesh *>(x))

class DUSK_OPENGL_API OpenGLMesh : public Mesh 
{
public:

    DISALLOW_COPY_AND_ASSIGN(OpenGLMesh)

    OpenGLMesh() = default;

    virtual ~OpenGLMesh() = default;

    void Render() override;

    bool Load(const MeshData * data) override;

private:

    GLenum GetGLMode(const MeshData::Mode& mode);

    GLuint _glVAO;
    GLenum _glMode;
    GLsizei _glCount;

    bool _indexed;

}; // class OpenGLMesh

} // namespace Dusk::OpenGL

#endif // DUSK_OPENGL_MESH_HPP
