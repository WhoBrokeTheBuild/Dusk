#ifndef DUSK_OPENGL_MESH_HPP
#define DUSK_OPENGL_MESH_HPP

#include <Dusk/OpenGL/OpenGLConfig.hpp>

#include <Dusk/Mesh.hpp>

namespace Dusk::OpenGL {

class DUSK_OPENGL_API OpenGLMesh : public Mesh
{
public:

    DISALLOW_COPY_AND_ASSIGN(OpenGLMesh)

    OpenGLMesh() = default;

    virtual ~OpenGLMesh() = default;

    void Render(RenderContext * ctx) override;

private:

}; // class OpenGLMesh

} // namespace Dusk::OpenGL

#endif // DUSK_OPENGL_MESH_HPP