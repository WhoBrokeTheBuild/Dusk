#ifndef DUSK_OPENGL_MATERIAL_HPP
#define DUSK_OPENGL_MATERIAL_HPP

#include <Dusk/OpenGL/OpenGLConfig.hpp>

#include <Dusk/Material.hpp>

namespace Dusk::OpenGL {

#define DUSK_OPENGL_MATERIAL(x) (dynamic_cast<Dusk::OpenGL::OpenGLMaterial *>(x))

class DUSK_OPENGL_API OpenGLMaterial : public Material
{
public:

    DISALLOW_COPY_AND_ASSIGN(OpenGLMaterial)

    OpenGLMaterial() = default;

    virtual ~OpenGLMaterial() = default;

    void Bind();

private:

}; // class OpenGLMaterial

} // namespace Dusk::OpenGL

#endif // DUSK_OPENGL_MATERIAL_HPP