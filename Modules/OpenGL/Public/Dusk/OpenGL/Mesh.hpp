#ifndef DUSK_OPENGL_MESH_HPP
#define DUSK_OPENGL_MESH_HPP

#include <Dusk/OpenGL/Config.hpp>
#include <Dusk/Graphics/Mesh.hpp>

namespace Dusk::OpenGL {

class DUSK_OPENGL_API Mesh : public IMesh 
{
public:

    void Render() override;

    bool Load(const IMeshData * data) override;

private:

    GLuint _glVAO;
    GLenum _glMode;
    GLsizei _glVertexCount;

    bool _indexed;

}; // class Mesh

} // namespace Dusk::OpenGL

#endif // DUSK_OPENGL_MESH_HPP
