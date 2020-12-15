#ifndef DUSK_OPENGL_PRIMITIVE_HPP
#define DUSK_OPENGL_PRIMITIVE_HPP

#include <Dusk/OpenGL/OpenGLConfig.hpp>

#include <Dusk/Primitive.hpp>
#include <Dusk/PrimitiveData.hpp>

namespace Dusk::OpenGL {

#define DUSK_OPENGL_PRIMITIVE(x) (dynamic_cast<Dusk::OpenGL::OpenGLPrimitive *>(x))

class DUSK_OPENGL_API OpenGLPrimitive : public Primitive 
{
public:

    DISALLOW_COPY_AND_ASSIGN(OpenGLPrimitive)

    OpenGLPrimitive() = default;

    virtual ~OpenGLPrimitive() = default;

    void Render();

    bool Load(const std::unique_ptr<PrimitiveData>& data) override;

private:

    GLenum GetGLMode(const Primitive::Topology& topology);

    GLuint _glVAO;

    GLenum _glMode;
    
    GLsizei _glCount;

    bool _indexed;

}; // class OpenGLPrimitive

} // namespace Dusk::OpenGL

#endif // DUSK_OPENGL_PRIMITIVE_HPP
