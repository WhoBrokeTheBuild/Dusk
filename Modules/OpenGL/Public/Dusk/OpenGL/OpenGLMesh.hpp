#ifndef DUSK_OPENGL_MESH_HPP
#define DUSK_OPENGL_MESH_HPP

#include <Dusk/OpenGL/OpenGLConfig.hpp>

#include <Dusk/Mesh.hpp>
#include <Dusk/OpenGL/OpenGLPrimitive.hpp>

namespace Dusk::OpenGL {

#define DUSK_OPENGL_MESH(x) (dynamic_cast<Dusk::OpenGL::OpenGLMesh *>(x))

class DUSK_OPENGL_API OpenGLMesh : public Mesh 
{
public:

    DISALLOW_COPY_AND_ASSIGN(OpenGLMesh)

    OpenGLMesh() = default;

    virtual ~OpenGLMesh() = default;

    void Render() override;

    bool Load(const std::vector<std::unique_ptr<PrimitiveData>>& data) override;

private:

    std::vector<std::unique_ptr<OpenGLPrimitive>> _primitives;

}; // class OpenGLMesh

} // namespace Dusk::OpenGL

#endif // DUSK_OPENGL_MESH_HPP
