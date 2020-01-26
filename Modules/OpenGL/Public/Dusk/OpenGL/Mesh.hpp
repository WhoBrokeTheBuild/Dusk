#ifndef DUSK_OPENGL_MESH_HPP
#define DUSK_OPENGL_MESH_HPP

#include <Dusk/OpenGL/Config.hpp>
#include <Dusk/Graphics/Mesh.hpp>

namespace Dusk::OpenGL {

class DUSK_OPENGL_API Mesh : public Dusk::Mesh 
{
public:

    void Render() override;

    bool Load(const MeshData * data) override;

private:

    GLenum GetGLMode(const MeshData::Mode& mode);

    GLuint _glVAO;
    GLenum _glMode;
    GLsizei _glCount;

    bool _indexed;

}; // class Mesh

} // namespace Dusk::OpenGL

#endif // DUSK_OPENGL_MESH_HPP
